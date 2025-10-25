#!/bin/sh

# Check if curl is available
if ! command -v curl >/dev/null 2>&1; then
  echo "Error: curl is not installed in the container."
  exit 1
fi

# Wait for Elasticsearch to be ready
attempt=0
until curl -s -u "${ELASTIC_USER}:${ELASTIC_PASSWORD}" http://elasticsearch:9200/_cluster/health | grep -q '"status":"green"\|"status":"yellow"'; do
  if [ $attempt -ge 12 ]; then
    echo "Error: Elasticsearch not ready after 12 attempts."
    exit 1
  fi
  attempt=$((attempt + 1))
  echo "Waiting for Elasticsearch to be ready... (attempt: ${attempt})"
  sleep 10
done

# Apply ILM policy with retry
echo "Attempting to apply ILM policy..."
attempt=0
max_attempts=30
until [ "$attempt" -ge "$max_attempts" ]
do
    response=$(curl -s -w "\n%{http_code}" -X PUT "http://elasticsearch:9200/_ilm/policy/pong_retention_policy" \
      -u "${ELASTIC_USER}:${ELASTIC_PASSWORD}" \
      -H 'Content-Type: application/json' \
      -d @/retention-policy.json)
    http_code=$(echo "$response" | tail -n1)
    response_body=$(echo "$response" | sed -e '$d')

    if [ "$http_code" -eq 200 ] && echo "$response_body" | grep -q '"acknowledged":true'; then
      echo "ILM policy applied successfully."
      break
    fi
    attempt=$((attempt+1))
    echo "Failed to apply ILM policy (attempt $attempt/$max_attempts). Retrying in 10s..."
    sleep 10
done

if [ "$attempt" -ge "$max_attempts" ]; then
  echo "Error: Failed to apply ILM policy after $max_attempts attempts."
  exit 1
fi

# Create index template for pong-logs-* with retry
echo "Attempting to create index template..."
attempt=0
until [ "$attempt" -ge "$max_attempts" ]
do
    response=$(curl -s -w "\n%{http_code}" -X PUT "http://elasticsearch:9200/_index_template/pong_logs_template" \
      -u "${ELASTIC_USER}:${ELASTIC_PASSWORD}" \
      -H 'Content-Type: application/json' \
      -d '{
        "index_patterns": ["pong-logs-*"],
        "template": {
          "settings": {
            "index.lifecycle.name": "pong_retention_policy"
          }
        }
      }')
    http_code=$(echo "$response" | tail -n1)
    response_body=$(echo "$response" | sed -e '$d')

    if [ "$http_code" -eq 200 ] && echo "$response_body" | grep -q '"acknowledged":true'; then
      echo "Index template created successfully."
      break
    fi
    attempt=$((attempt+1))
    echo "Failed to create index template (attempt $attempt/$max_attempts). Retrying in 10s..."
    sleep 10
done

if [ "$attempt" -ge "$max_attempts" ]; then
  echo "Error: Failed to create index template after $max_attempts attempts."
  exit 1
fi

echo "ILM setup completed."
#!/bin/sh

echo "Waiting for Kibana to be ready..."
max_attempts=60
attempt=1

while [ $attempt -le $max_attempts ]; do
    if curl -s -u ${ELASTIC_USER}:${ELASTIC_PASSWORD}  http://kibana:5601/api/status | grep -q "available"; then
        echo "Kibana is ready!"
        break
    fi
    echo "Attempt $attempt/$max_attempts - Kibana not ready yet..."
    sleep 5
    attempt=$((attempt + 1))
done

if [ $attempt -gt $max_attempts ]; then
    echo "Kibana failed to become ready"
    exit 1
fi

echo "Importing dashboards..."
attempt=0
max_attempts=12
until [ "$attempt" -ge "$max_attempts" ]
do
    response=$(curl -s -u ${ELASTIC_USER}:${ELASTIC_PASSWORD} -w "\nHTTP_STATUS:%{http_code}" -X POST "http://kibana:5601/api/saved_objects/_import?overwrite=true" \
        -H "kbn-xsrf: true" \
        --form file=@/dashboards.ndjson)

    http_status=$(echo "$response" | tail -n1 | cut -d: -f2)
    body=$(echo "$response" | sed '$d')

    if [ "$http_status" = "200" ]; then
        echo "Dashboard import successful!"
        exit 0
    fi
    
    attempt=$((attempt+1))
    echo "Dashboard import failed with status $http_status (attempt $attempt/$max_attempts). Retrying in 10s..."
    echo "Response body: $body"
    sleep 10
done

echo "Error: Dashboard import failed after $max_attempts attempts."
exit 1
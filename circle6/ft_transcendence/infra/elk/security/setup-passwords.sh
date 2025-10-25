#!/bin/bash
set -e

echo "Setting up ELK Security..."

until curl -s -u ${ELASTIC_USER}:${ELASTIC_PASSWORD} http://elasticsearch:9200/_cluster/health > /dev/null 2>&1; do
  echo "Waiting for Elasticsearch..."
  sleep 5
done

echo "Elasticsearch ready!"

echo "Setting kibana_system password..."
curl -s -X POST -u ${ELASTIC_USER}:${ELASTIC_PASSWORD} -H "Content-Type: application/json" \
  http://elasticsearch:9200/_security/user/${KIBANA_USER}/_password \
  -d "{\"password\":\"${KIBANA_PASSWORD}\"}"

echo "Creating logstash_writer role..."
curl -s -X PUT -u ${ELASTIC_USER}:${ELASTIC_PASSWORD} -H "Content-Type: application/json" \
  http://elasticsearch:9200/_security/role/logstash_writer \
  -d '{"cluster":["manage_index_templates","monitor","manage_ilm"],"indices":[{"names":["pong-logs-*","logstash-*"],"privileges":["write","create","create_index","manage","manage_ilm"]}]}'

echo "Creating logstash_internal user..."
curl -s -X POST -u ${ELASTIC_USER}:${ELASTIC_PASSWORD} -H "Content-Type: application/json" \
  http://elasticsearch:9200/_security/user/${LOGSTASH_USER} \
  -d "{\"password\":\"${LOGSTASH_PASSWORD}\",\"roles\":[\"logstash_writer\"]}"

echo "Security setup complete!"
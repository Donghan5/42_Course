#!/bin/bash

echo "🚀 Guide of Google OAuth settings"

# Take the IP address
IP_MODE="private"
if [ "$IP_MODE" = "private" ]; then
    HOST_IP=$(hostname -I | awk '{print $1}')
else
    HOST_IP=$(curl -s ifconfig.me)
fi

echo "✅ Detected IP: ${HOST_IP}"
echo ""
echo "📝 those URL should add by yourself:"
echo "------------------------------------------------------------"
echo "1. Enter Google Cloud Console:"
echo "   https://console.cloud.google.com/apis/credentials"
echo ""
echo "2. Click OAuth 2.0 client ID"
echo ""
echo "3. Add to Authorized JavaScript origins:"
echo "   ➡️  https://transcendence.${HOST_IP}.nip.io:8443"
echo ""
echo "4. Add to Authorized redirect URIs:"
echo "   ➡️  https://transcendence.${HOST_IP}.nip.io:8443/api/users/login/google/callback"
echo "------------------------------------------------------------"
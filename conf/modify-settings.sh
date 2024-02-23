#!/bin/bash
set -e

# Check if the modification has already been made
if ! grep -q "getenv('DRUPAL_BASE_URL')" /var/www/html/sites/default/default.settings.php; then
  # Modify the settings.php file
  sed -i "s|# \$base_url = 'http:\/\/www\.example\.com';  // NO trailing slash\!|\$base_url = getenv('DRUPAL_BASE_URL') \?: 'http:\/\/localhost:8787';|g" /var/www/html/sites/default/default.settings.php
fi

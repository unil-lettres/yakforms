FROM drupal:7-php7.4-fpm-bullseye

# Install needed PHP extensions
RUN apt update && apt install -y libldap2-dev && docker-php-ext-install ldap

# https://framagit.org/yakforms/yakforms
ARG YAK_COMMIT=4081c97043d923c8fdf285fc17cae62c48a78c5a

# Install Yakforms profile
RUN curl https://framagit.org/yakforms/yakforms/-/archive/$YAK_COMMIT/yakforms-$YAK_COMMIT.tar.gz?path=profiles/yakforms_profile -o /tmp/yakforms-$YAK_COMMIT.tar.gz && \
    tar -xvf /tmp/yakforms-$YAK_COMMIT.tar.gz --strip-components=2 -C /var/www/html/profiles/ && \
    rm -r /tmp/*

# Replace files with issues by fixed ones
COPY ./fix/page.tpl.php /var/www/html/profiles/yakforms_profile/themes/yaktheme/templates/page.tpl.php
COPY ./fix/yakforms.admin.inc /var/www/html/profiles/yakforms_profile/modules/yakforms/includes/yakforms.admin.inc
COPY ./fix/Form.php /var/www/html/profiles/yakforms_profile/modules/form_builder/modules/webform/src/Form.php
COPY ./fix/file.inc /var/www/html/profiles/yakforms_profile/modules/webform/components/file.inc
COPY ./fix/webform_charts.module /var/www/html/profiles/yakforms_profile/modules/webform_charts/webform_charts.module
COPY ./fix/bootstrap.inc /var/www/html/includes/bootstrap.inc

# Replace homepage templates
COPY ./views/frontpage-fr.html /var/www/html/profiles/yakforms_profile/modules/yakforms/includes/html/fr/frontpage.html
COPY ./views/frontpage-en.html /var/www/html/profiles/yakforms_profile/modules/yakforms/includes/html/en/frontpage.html

FROM drupal:7-php7.4-fpm-bullseye

# https://framagit.org/yakforms/yakforms
ENV YAK_COMMIT=4081c97043d923c8fdf285fc17cae62c48a78c5a

ENV COMPOSER_VERSION=2.6
ENV PSQL_CLIENT_VERSION=16
# Drush 8.x is the latest Drupal 7 compatible version
ENV DRUSH_VERSION=8.4

# Install needed packages
RUN apt update &&  \
    apt install -y \
    nano \
    curl \
    wget \
    unzip \
    lsb-release \
    gnupg2 \
    libldap2-dev

# Install specific version of the PostgreSQL client
RUN sh -c 'echo "deb http://apt.postgresql.org/pub/repos/apt $(lsb_release -cs)-pgdg main" > /etc/apt/sources.list.d/pgdg.list' && \
    curl -fsSL https://www.postgresql.org/media/keys/ACCC4CF8.asc | gpg --dearmor -o /etc/apt/trusted.gpg.d/postgresql.gpg && \
    apt update && \
    apt install -y postgresql-client-$PSQL_CLIENT_VERSION

# Install specific version of Composer
RUN curl --silent --show-error https://getcomposer.org/installer | php -- \
    --$COMPOSER_VERSION \
    --install-dir=/usr/local/bin --filename=composer && \
    ln -s /root/.composer/vendor/bin/drush /usr/local/bin/drush

# Install specific version of Drush
RUN composer global require drush/drush:$DRUSH_VERSION && \
    composer global update

# Install needed php extensions
RUN apt-get clean; docker-php-ext-install ldap

# Install Yakforms profile
RUN curl https://framagit.org/yakforms/yakforms/-/archive/$YAK_COMMIT/yakforms-$YAK_COMMIT.tar.gz?path=profiles/yakforms_profile -o /tmp/yakforms-$YAK_COMMIT.tar.gz && \
    tar -xvf /tmp/yakforms-$YAK_COMMIT.tar.gz --strip-components=2 -C /var/www/html/profiles/ && \
    rm -r /tmp/*

# Replace files with issues by fixed ones
COPY ./fix/page.tpl.php /var/www/html/profiles/yakforms_profile/themes/yaktheme/templates/page.tpl.php
COPY ./fix/yakforms.admin.inc /var/www/html/profiles/yakforms_profile/modules/yakforms/includes/yakforms.admin.inc
COPY ./fix/yakforms.module /var/www/html/profiles/yakforms_profile/modules/yakforms/yakforms.module
COPY ./fix/Form.php /var/www/html/profiles/yakforms_profile/modules/form_builder/modules/webform/src/Form.php
COPY ./fix/file.inc /var/www/html/profiles/yakforms_profile/modules/webform/components/file.inc
COPY ./fix/webform_charts.module /var/www/html/profiles/yakforms_profile/modules/webform_charts/webform_charts.module
COPY ./fix/yaktheme.js /var/www/html/profiles/yakforms_profile/themes/yaktheme/js/yaktheme.js
COPY ./fix/cleaning.css /var/www/html/profiles/yakforms_profile/themes/yaktheme/css/cleaning.css
COPY ./fix/header.css /var/www/html/profiles/yakforms_profile/themes/yaktheme/css/header.css
COPY ./fix/bootstrap.inc /var/www/html/includes/bootstrap.inc

# Replace homepage templates
COPY ./views/frontpage-fr.html /var/www/html/profiles/yakforms_profile/modules/yakforms/includes/html/fr/frontpage.html
COPY ./views/frontpage-en.html /var/www/html/profiles/yakforms_profile/modules/yakforms/includes/html/en/frontpage.html

# Create directory to store uploaded form files
RUN mkdir -p /var/www/html/sites/default/files/forms/
RUN chown -R www-data:www-data /var/www/html/sites/default/files/

# Copy the script to modify the default Drupal settings into the image, make it executable and run it
COPY ./conf/modify-settings.sh /modify-settings.sh
RUN chmod +x /modify-settings.sh
RUN /modify-settings.sh

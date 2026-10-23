# Use the existing Drupal image as the source of application files
FROM drupal:7-php7.4-fpm-bullseye AS drupal-base

# Use PHP 7.4 backports with CVE-2026-109618 and CVE-2026-109619 fixes
FROM ghcr.io/mpyw/php-eol-cve-2026-109618-109619:7.4-fpm-bullseye

# https://framagit.org/yakforms/yakforms
ENV YAK_COMMIT=4081c97043d923c8fdf285fc17cae62c48a78c5a

ENV COMPOSER_VERSION=2.2.28
ENV PSQL_CLIENT_VERSION=16
# Drush 8.x is the latest Drupal 7 compatible version
ENV DRUSH_VERSION=8.4

# Copy Drupal from the source image and restore ownership
COPY --from=drupal-base /var/www/html/ /var/www/html/
RUN chown -R www-data:www-data \
    /var/www/html/sites \
    /var/www/html/modules \
    /var/www/html/themes

# Update repositories and install additional packages
RUN set -eux; \
    apt-get -o Acquire::Check-Valid-Until=false update; \
    apt-get install -y --no-install-recommends \
        nano \
        curl \
        wget \
        unzip \
        lsb-release \
        gnupg \
        libldap2-dev \
        libfreetype6-dev \
        libjpeg62-turbo-dev \
        libpng-dev \
        libwebp-dev \
        libzip-dev \
        libpq-dev \
        zlib1g-dev; \
    rm -rf /var/lib/apt/lists/*

# Install required PHP extentions
RUN set -eux; \
    docker-php-ext-configure gd \
        --with-freetype \
        --with-jpeg=/usr \
        --with-webp; \
    docker-php-ext-configure ldap \
        --with-libdir="lib/$(dpkg-architecture --query DEB_BUILD_MULTIARCH)"; \
    docker-php-ext-install -j"$(nproc)" \
        gd \
        opcache \
        pdo_mysql \
        pdo_pgsql \
        zip \
        ldap

# Install the specific PostgreSQL client version, use the archived PGDG repository for Bullseye
RUN set -eux; \
    echo "deb https://apt-archive.postgresql.org/pub/repos/apt $(lsb_release -cs)-pgdg main" \
        > /etc/apt/sources.list.d/pgdg.list; \
    curl -fsSL https://www.postgresql.org/media/keys/ACCC4CF8.asc \
        | gpg --dearmor -o /etc/apt/trusted.gpg.d/postgresql.gpg; \
    apt-get -o Acquire::Check-Valid-Until=false update; \
    apt-get install -y --no-install-recommends \
        "postgresql-client-${PSQL_CLIENT_VERSION}"; \
    rm -rf /var/lib/apt/lists/*

# Install the specific Composer version
RUN set -eux; \
    curl --silent --show-error https://getcomposer.org/installer \
        | php -- \
            --version="${COMPOSER_VERSION}" \
            --install-dir=/usr/local/bin \
            --filename=composer; \
    ln -s /root/.composer/vendor/bin/drush /usr/local/bin/drush

# Install the specific Drush version
RUN set -eux; \
    composer global require "drush/drush:${DRUSH_VERSION}"; \
    composer global update

# Recommended OPcache settings for Drupal
RUN set -eux; \
    { \
        echo 'opcache.memory_consumption=128'; \
        echo 'opcache.interned_strings_buffer=8'; \
        echo 'opcache.max_accelerated_files=4000'; \
        echo 'opcache.revalidate_freq=60'; \
    } > /usr/local/etc/php/conf.d/opcache-recommended.ini

# Increase the upload file size
COPY conf/php-uploads.ini /usr/local/etc/php/conf.d/uploads.ini

# Install the Yakforms profile
RUN set -eux; \
    curl -fsSL \
        "https://framagit.org/yakforms/yakforms/-/archive/${YAK_COMMIT}/yakforms-${YAK_COMMIT}.tar.gz?path=profiles/yakforms_profile" \
        -o "/tmp/yakforms-${YAK_COMMIT}.tar.gz"; \
    tar -xzf "/tmp/yakforms-${YAK_COMMIT}.tar.gz" \
        --strip-components=2 \
        -C /var/www/html/profiles/; \
    rm -f "/tmp/yakforms-${YAK_COMMIT}.tar.gz"

# Replace files with fixes
COPY fix/yakforms.admin.inc /var/www/html/profiles/yakforms_profile/modules/yakforms/includes/yakforms.admin.inc
COPY fix/yakforms.module /var/www/html/profiles/yakforms_profile/modules/yakforms/yakforms.module
COPY fix/Form.php /var/www/html/profiles/yakforms_profile/modules/form_builder/modules/webform/src/Form.php
COPY fix/file.inc /var/www/html/profiles/yakforms_profile/modules/webform/components/file.inc
COPY fix/webform_charts.module /var/www/html/profiles/yakforms_profile/modules/webform_charts/webform_charts.module
COPY fix/bootstrap.inc /var/www/html/includes/bootstrap.inc
COPY fix/riddler.install /var/www/html/profiles/yakforms_profile/modules/riddler/riddler.install
COPY fix/webform_charts.tokens.inc /var/www/html/profiles/yakforms_profile/modules/webform_charts/webform_charts.tokens.inc

# Replace homepage templates
COPY views/frontpage-fr.html /var/www/html/profiles/yakforms_profile/modules/yakforms/includes/html/fr/frontpage.html
COPY views/frontpage-en.html /var/www/html/profiles/yakforms_profile/modules/yakforms/includes/html/en/frontpage.html

# Create directories for uploaded form files
RUN set -eux; \
    mkdir -p \
        /var/www/html/sites/default/files/forms \
        /var/www/html/sites/default/files/webform; \
    chown -R www-data:www-data /var/www/html/sites/default/files

# Modify the default Drupal settings.
COPY conf/modify-settings.sh /modify-settings.sh
RUN chmod +x /modify-settings.sh \
    && /modify-settings.sh

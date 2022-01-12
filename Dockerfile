FROM drupal:7

ARG VERSION=develop

RUN curl https://framagit.org/yakforms/yakforms/-/archive/$VERSION/yakforms-$VERSION.tar.gz?path=profiles/yakforms_profile -o /tmp/yakforms-$VERSION.tar.gz && \
    tar -xvf /tmp/yakforms-$VERSION.tar.gz --strip-components=2 -C /var/www/html/profiles/ && \
    rm -r /tmp/*

# Information

Docker configuration for the [Yakforms](https://framagit.org/yakforms/yakforms) project. This is a Drupal 7 / PHP 7.4 application that lets you create simple forms, then collect and analyze the responses.

It is not actively maintained anymore, we'll keep the stack frozen until a better solution is found.
- https://framacolibri.org/t/yakforms-migration-sur-drupal-9/17286
- https://framacolibri.org/t/updating-an-outdated-technology-stack-and-feature-timeline/17166

# Development with Docker

## Docker installation

A working [Docker](https://docs.docker.com/engine/install/) installation is mandatory.

## Docker environment file

Please make sure to copy & rename the **example.env** file to **.env**.

``cp example.env .env``

You can replace the values if needed to match you server & environment.

## Build & run

Build & run all the containers for this project.

``docker-compose up`` (add -d if you want to run in the background and silence the logs)

## Frontend

To access the main application please use the following link.

http://localhost:8787

The first time you access the application, you will be redirected to the installation page (see below for the installation instructions).

# Deployment with Docker

Copy & rename the **docker-compose.override.yml.prod** file to **docker-compose.override.yml**.

`cp docker-compose.override.yml.prod docker-compose.override.yml`

You can replace the values if needed, but the default ones should work for production.

Don't forget to also copy & rename the **example.env** file to **.env**, and change the values for the production environment.

Build & run all the containers for this project:

`docker-compose up -d`

Use a reverse proxy configuration to map the url to port `8787`.

# Installation instructions

When you first access the application, you will be redirected to the installation page.

## 1. Database configuration
- Choose PostgreSQL as the database type
- Add connection parameters (report values from the .env file, below are the default development values)
  - Database name: yakforms
  - Username: user
  - Password: password
  - Host: yakforms-postgres
  - Port: 5432

## 2. Then, configure the admin account

## 3. Add French & make it the default language

Use the following path: `/admin/config/regional/language`

## 4. Activate Yakforms modules

Use the following path: `/admin/modules`

- "yakforms-feature"
- "Yakforms"
- "Yakforms Public Results" & "Yakforms Share Results"

## 5. Activate the Yakforms "Lettres" theme, make it the default & use it also for admin

Use the following path: `/admin/appearance`

## 6. Update general information & create default pages

Use the following path: `/admin/config/system/yakforms`

## 8. Activate & configure modules

Use the following path: `/admin/modules`

- "SMTP Authentication Support"
- If needed also "Active Directory Integration / LDAP Integration - NTLM & Kerberos Login"

# Update instructions

Everything in `/themes`, `/modules`, `/libraries` & `/files` is managed by composer and will be updated on the next `docker compose up`.

Files in the `/fix` & `/views` folders are backed into the docker image, they will either be updated on the next `docker compose build` / `docker compose pull`, or stored in the database during the first setup and persist in the application database volume (should be updated manually).

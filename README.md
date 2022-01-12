# Installation
## Manual build (optionnal)

This will build 3 flavors of docker images :
- yakforms:latest
- yakforms:latest-fpm
- yakforms:latest-fpm-alpine

fpm version is needed to run with nginx, in docker-compose.yml.

fpm-alpine is a lightweight alternative.

latest is currently useless but it's vanilla drupal, which might be useful to someone, I don't know ¯\_(ツ)_/¯


Clone this repo and run 
```
chmod +x docker-build.sh
./docker-build.sh
```

To change Yakforms version, edit `docker-build.sh`.

## Run

If you have skipped manual build and didn't clone this repo, download required files :
```
mkdir -p yakforms/nginx && cd yakforms/
curl -L -o nginx/default.conf "https://framagit.org/gnouts/yakforms-docker/-/raw/master/conf/nginx/nginx_default.conf"
curl -L -o docker-compose.yml "https://framagit.org/gnouts/yakforms-docker/-/raw/master/docker-compose.yml"
```

Then, download the default environment file :
```
curl -L -o .env "https://framagit.org/gnouts/yakforms-docker/-/raw/master/env.prod.sample"
```

Edit the configuration file `.env`

Launch yakforms :
```
docker-compose up -d
```


## First setup

By default, yakforms will be available on http://localhost:80/ (you can set a proxy in front if needed).

You will need to complete first setup of Drupal and provided some information :

- visit http://localhost:80/
- (yakforms_profile is automatically selected (look at your url), so you should already be on the second step ("Choose language"), otherwise select yakforms_profile and continue)
- Select "Postgres" as database type. Fill next fields with the value you set in the `.env` file.
- Click "Advanced" to reveal the field "Database host" and fill it with `db`. Save and continue.
- Once Drupal installation finished, click on "Modules", scroll to the bottom and select `yakforms-feature`. Click "Save configuration" to install it (and accept all dependencies modules)
- Then, go back to "Modules", select `yakforms`, "Save configuration".

You have a basic Yakforms installation.

Then you can install other yakform modules using same steps as above.

And configure email and such, following official documentation.

# Update

Change `YAKFORMS_VERSION` in your `.env` file.

```
docker-compose up -d
```

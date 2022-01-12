# Installation
## Run

```
mkdir -p yakforms/nginx
curl -L -o nginx/default.conf "https://framagit.org/gnouts/yakforms-docker/-/raw/master/conf/nginx/nginx_default.conf"
curl -L -o .env "https://framagit.org/gnouts/yakforms-docker/-/raw/master/env.prod.sample"
curl -L -o docker-compose.yml "https://framagit.org/gnouts/yakforms-docker/-/raw/master/docker-compose.yml"
```
(or clone this whole repo)


Edit the configuration file `.env`

Launch yakforms :
```
docker-compose up -d
```

## First setup

By default, yakforms will be available on http://localhost:80/ (you can set a proxy in front if needed).

You will need to complete first setup of Drupal and provided some information :

- visit http://localhost:80/
- (yakforms_profile is automatically selected, so you should already be on the second step, otherwise select yakforms_profile)
- Select "Postgres" as database type. Fill next fields with the value you set in the `.env` file.
- Click "Advanced" to reveal the field "Database host" and fill it 'postgres'. Save and continue.



# Update

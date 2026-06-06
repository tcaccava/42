# Project Backend

This folder contains the backend code of project microservice. This microservice serves project data to the gateway, as well as manages project archive and screenshot uploads and management.

Note that this document contains notes I've written for myself (Ihor). To view documentation, visit [here](DOCS.md)

## Running this thing

Just run `npm start` for a simple run. For development, it is preferred to use `npm run start:dev`, as it will monitor and immediately update changes made to the source code.

## Prisma

This backend uses `Prisma` as a database connector and ORM solution. To manage local database, run `npx prisma dev`.

[More info here.](https://docs.nestjs.com/recipes/prisma)

## Garage

Garage will be our S3 storage system backend. To start it up, go to the `bin` directory, and run the following command:

```
garage -c ./garage.toml server
```

Preparing Garage for deployment is a bit of a pain in the ass. Here are the steps I did that I'll have to script in the future:

```bash
./garage -c garage.toml status # to retrieve our `node_id`
./garage -c garage.toml layout assign -z alpha -c 1G <node_id> # where `alpha` is simply the name of the "zone", and <node_id> is the node ID we have retrieved previously
./garage -c garage.toml layout apply --version 1 # to apply previous changes (the "--version 1" part is necessary, and should increase if we were to change layout in the future, I presume)
./garage -c garage.toml bucket create transcendence # where `transcendence` is the name of the bucket
./garage -c garage.toml key create transcendence-app-key # will create the keys and secrets we will need to access our bucket. It will return these to stdout, and we will have to extract those somehow
./garage -c garage.toml bucket allow --read --write --owner transcendence --key transcendence-app-key # glues it all together
./garage -c garage.toml bucket website transcendence --allow # exposes the bucket as a website, allowing direct URLs to its resources (we likely don't need it)
```

[Garage docs.](https://garagehq.deuxfleurs.fr/documentation/quick-start/)

## Testing file uploads

Some API endpoints may require files to be uploaded via forms. I haven't found a good API client to test this functionality with, so cURL will suffice:

```bash
curl -v -F file=@cool.txt -F file2=@awesome.txt http://127.0.0.1:3000/projects
```

Here, `@cool.txt` and `@awesome.txt` point to local files, and will be uploaded as parts of the form ([thank SO!](https://stackoverflow.com/questions/19116016/what-is-the-right-way-to-post-multipart-form-data-using-curl)).


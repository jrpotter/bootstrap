# NiFi Dev Shell

This is a small flake template for experimenting with [Apache NiFi](https://nifi.apache.org/)
(version 1.19.0). [direnv](https://direnv.net/) can be used to a launch a dev
shell upon entering this directory (refer to `.envrc`). Otherwise run via:
```bash
$ nix develop
```
On account of hard-coded relative paths and expectations around file
permissions, NiFi is not very Nix-friendly. Instead of making NiFi directly
accessible from the shell, we instead use Nix to create a docker image that
can then be used to boot NiFi. Do so by running:
```bash
$ nix build
$ docker load < result
$ docker run -p 8443:8443 nifi:1.19.0
```
Once running, open `https://localhost:8443/nifi` (notice use of the `https`
scheme). You can find your login credentials using:
```bash
$ docker exec -it <container-id> grep Generated /opt/nifi/nifi-current/logs/nifi-app.log
```

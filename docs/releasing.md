# Releasing

Pushing a `v*` tag runs `.github/workflows/release.yml`, which builds Windows and Linux packages and publishes them as a GitHub Release.

## Steps

1. Bump the version in `CMakeLists.txt`:
   ```cmake
   project(soundbored VERSION 0.2.0
   ```
2. Merge that change into `main` through a PR.
3. Tag the merged commit and push the tag:
   ```sh
   git switch main && git pull
   git tag v0.2.0
   git push origin v0.2.0
   ```
4. Watch the run in the Actions tab. The release appears on the Releases page when it's green.

The tag must match the CMake version (`v0.2.0` ↔ `0.2.0`), or the workflow fails before building.

## What gets published

- `soundbored-<version>-windows-x64.zip`
- `soundbored-<version>-linux-x64.tar.gz`
- `SHA256SUMS`
- Release notes generated from the PRs merged since the previous tag

Each archive holds a `soundbored/` folder with the executable, `assets/` and `LICENSE`.

## Testing without publishing

- Any PR that touches `release.yml` or `CMakeLists.txt` builds the packages.
- Or run the workflow by hand: Actions tab → Release → Run workflow.

Either way, the packages are attached to the run as artifacts. Nothing is published.

## Fixing a bad release

Delete the release and the tag, fix, then tag again:

```sh
gh release delete v0.2.0 --cleanup-tag
```

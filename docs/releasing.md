# Releasing

Pushing a `v*` tag runs `.github/workflows/release.yml`, which builds Windows and Linux packages and publishes them as a GitHub Release.

## Steps

The tag is the version: there's no version number to bump in the code.

```sh
git switch main && git pull
git tag v0.2.0
git push origin v0.2.0
```

Then watch the run in the Actions tab. The release appears on the Releases page when it's green.

The workflow fails before building if:
- the tag isn't `vX.Y.Z` (e.g. `v0.2`, `vtest`)
- the tagged commit isn't on `main`. Tag only from an up-to-date `main`

Only admins can create, move or delete `v*` tags (tag ruleset in the repo settings).

## What gets published

- `soundbored-<version>-windows-x64.zip`
- `soundbored-<version>-linux-x64.tar.gz`
- `SHA256SUMS`
- Release notes generated from the PRs merged since the previous tag

Each archive holds a `soundbored/` folder with the executable, `assets/` and `LICENSE`.

## Testing without publishing

- Any PR that touches `release.yml` or `CMakeLists.txt` builds the packages.
- Or run the workflow by hand: Actions tab → Release → Run workflow.

Either way, the packages are attached to the run as artifacts, versioned `dev-<commit>`. Nothing is published.

## Fixing a bad release

Delete the release and the tag, fix, then tag again:

```sh
gh release delete v0.2.0 --cleanup-tag
```

A tag pushed by mistake that didn't publish anything (its run failed the checks):

```sh
git push origin --delete v0.2.0
git tag -d v0.2.0
```

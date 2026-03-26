# Versioning Scheme

Masonry uses a four-part version number:

```
Major.Minor.Patch.Build
```

## Components

| Component | Range | Meaning | Who bumps it |
|-----------|-------|---------|--------------|
| **Major** | 0-1 | `0` = pre-release, `1` = full release | Manual, once |
| **Minor** | 0-9 | `0` = private/internal, `1+` = public. Increments for major feature milestones (up to 9 before full release) | Manual |
| **Patch** | 0+ | Minor updates, improvements, new features | Manual |
| **Build** | 0+ | Bugs, tweaks, minor changes | Auto (CI) |

## Examples

| Version | Meaning |
|---------|---------|
| `0.0.1.0` | Early private development, first patch |
| `0.0.1.15` | Same patch, 15th CI build |
| `0.0.3.0` | Third round of improvements, still private |
| `0.1.0.0` | First public release |
| `0.1.2.8` | Public, second patch, 8th build |
| `0.5.0.0` | Halfway to full release |
| `1.0.0.0` | Full release |

## How it works

- **CMakeLists.txt** defines the base version as `Major.Minor.Patch.0`
- **CI** (`.github/workflows/release.yml`) replaces the `.0` build component with `GITHUB_RUN_NUMBER` automatically
- **Release tags** are formatted as `vMajor.Minor.Patch.Build` (e.g., `v0.0.1.15`)
- **Local builds** show `.0` as the build number since they don't run through CI

## When to bump

- **Starting a new feature area?** Bump Patch: `0.0.1.0` -> `0.0.2.0`
- **Going public?** Bump Minor: `0.0.x.0` -> `0.1.0.0`
- **Major milestone with lots of new content?** Bump Minor: `0.1.0.0` -> `0.2.0.0`
- **Ready for full release?** Set Major to 1: `0.9.x.0` -> `1.0.0.0`
- **Bug fixes and tweaks?** Don't touch anything — the CI build number handles it

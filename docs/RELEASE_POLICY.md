# Dandelion OS Release Policy

This project keeps two records in parallel and never conflates them:

- the canonical project lineage in [`../VERSION_LOG.md`](../VERSION_LOG.md)
- the published release objects on the GitHub Releases page

## Rule 1: version log is the canonical history

[`../VERSION_LOG.md`](../VERSION_LOG.md) is the project's source of truth for milestones, architecture lineage, and current version identity.

## Rule 2: GitHub Releases are publish records

GitHub Releases are only the records for artifacts that have actually been published. A milestone may exist in the repository history without yet having a corresponding release object.

## Rule 3: do not claim a release that has not been published

Before saying a version is "released":

- confirm the Git tag exists
- confirm the target commit is correct
- confirm the release notes match the intended milestone
- confirm the artifacts are present and ready to share

## Rule 4: preserve the project's voice and history

Dandelion OS is intentionally expressive and human-centered. Do not flatten the project into sterile release boilerplate or erase earlier milestones. Keep the original ideas and accessibility-first ethos in place while making the public record easier to follow.

## Rule 5: keep the project honest

When the version log says a version is current but the GitHub Releases page does not show it yet, say that plainly. The fix is to publish the missing release, not to rewrite history or pretend the release already exists.

## Recommended workflow

1. update [`../VERSION_LOG.md`](../VERSION_LOG.md) for the milestone
2. update [`../CHANGELOG.md`](../CHANGELOG.md) with the milestone summary
3. verify the tag and target commit
4. publish the GitHub Release only when the notes and artifacts are ready
5. keep the release page aligned with the project story over time

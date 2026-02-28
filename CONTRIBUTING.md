# Contributing to SoftOmniSwift

Welcome, and thanks for your interest in contributing! SoftOmniSwift is being built
to last. Our goal is a high-quality, cohesive framework where all components share the
same underlying fabric. We're glad to have you here.

The best place to start is our [Discord](https://discord.gg/8Cx4amRK). Introduce
yourself, discuss your ideas, and get a feel for what's being worked on before diving
in. We'd rather talk through an idea upfront than have you spend time on something
that doesn't fit.

---

## Contributing

1. Open an issue and get it assigned to you before writing any code.
2. Branch off the relevant `dev/<subsystem>` integration branch.
3. Open a PR referencing the issue (`Closes #N`) when ready for review.

Every PR must reference at least one issue. PRs are for discussion, expect
collaboration and iteration before merge.

## AI-Generated Code

You are welcome to use AI tooling to assist with contributions. However, you are fully
responsible for everything you submit. If any part of your contribution was generated
or assisted by AI, disclose it in your PR description.

Do not submit AI-generated code you do not fully understand. Reviewers will ask you to
explain and defend your changes, and "the AI wrote it" is not an acceptable answer. The
same quality bar applies to all contributions regardless of how they were produced.

If you find yourself relying on AI because you're not yet familiar enough with the codebase or the relevant concepts to write the code yourself, that's a sign to take a step back. We encourage you to spend that time learning and practicing instead.
We're not against you contributing in the future once you've built up the experience, in fact we wholeheartedly welcome seeing people grow into the field and come back ready to contribute.

---

## Branch Naming

Integration branches are maintainer-owned: `dev/<subsystem>`

Your work lives in your own namespace, branched off the relevant integration branch:

```
<username>/<type>/<short-description>
```

Example: `dysax/docs/add-contributing`

---

## Commit Messages

Follow [Conventional Commits](https://www.conventionalcommits.org/):

```
<type>(<scope>): <short description>
```

| Type | Use for |
|------|---------|
| `feat` | New features |
| `fix` | Bug fixes |
| `docs` | Documentation |
| `chore` | Build system, tooling, maintenance |
| `refactor` | No behavior change |
| `test` | Tests |

Example: `docs(setup_and_install): add CONTRIBUTING.md`

---

## Opening a Pull Request

When you are ready, open a PR against the relevant `dev/<subsystem>` integration branch. PRs should never target `master` directly.

**Title format:**

```
<type>(<scope>): <short description> #<issue-number>
```

Example: `docs(setup_and_install): add CONTRIBUTING.md #1`

**Description format:**
```
## What
A summary of the changes in this PR.

## Why
The motivation behind the change and a link to the issue it addresses.

## Testing
How you verified the change works.

Closes #<issue-number>
```

The `Closes #N` line is important. GitHub will automatically close the linked issue
when the PR is merged, keeping things tidy without any manual cleanup.

A few things to keep in mind when opening a PR:
- Keep PRs focused. One issue per PR makes review easier and keeps history clean.
- If your PR is still in progress, open it as a draft so reviewers know it isn't ready.
- Be responsive to feedback. PRs that go quiet get stale and are harder to merge.

---

## Code Style

---

## In-Tree Contributions

We strongly encourage upstreaming over forking. If you want to add support for a
language or target, open an issue and discuss it with the maintainers. Quality
contributions are always welcome in-tree.

---

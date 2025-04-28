# SilvinoR's QMK Community Modules

> :bangbang: **NB:** These work on QMK Firmware 0.28.0 or later, merged in 2025q1.

Please follow the instructions at https://docs.qmk.fm/features/community_modules on how to use this.

i.e., set up should be something to the effect of:

```sh
cd /path/to/your/userspace
git submodule add https://github.com/silvinor/qmk_modules.git modules/silvinor
git submodule update --init --recursive
```

To update to the latest commit (of all submodules), use:

```sh
git submodule update --remote --merge
```

Each module is in it's own child folder, and has a README.md file with instructions on how to use it.

| Module | Description |
| --:|:-- |
| SR_Common | Some common code that was previously in my userspace |
| SR_Caffeine | Adds a screen saver blocker / keep alive key |
| SR_Version | Adds a keycode to print the compiled QMK version and date without the fluff |

> Sorry, individual READMEs are still in my TODO list :frowning:.

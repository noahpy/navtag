# navtag
Path shortcut manager CLI for Unix-like systems.

## Features
- List, add and remove shortcuts
- Translate shortcut labels to paths
- Translate paths including shortcut label to paths

## Quickstart
1. Clone this repository to any destination
2. Enter cloned repo and run:
```shell
./install.sh
```
## Useage
From here on, `$mfp` is equivalent to the path to the shortcut file.
### List shortcuts:
```shell
navtag $mfp -l
```
### Add shortcut:
```shell
navtag $mfp -a <label> <path>
```
### Remove shortcut:
```shell
navtag $mfp -d <label>
```
### Translate shortcut labels:
```shell
navtag $mfp -t [label]
```
## Shell extension
The following commands are implemented in commands.sh, using `navtag`.
The following list is a summary on the usage of every command:
- `marks`: list all shortcuts
- `mark <label>`: create a shortcut for current working directory with given label
- `unmark <label>`: remove shortcut with given label
- `cdx`: execute cd after translating every input arg
- `mvx`: execute mv after translating every input arg
- `cpx`: execute cp after translating every input arg

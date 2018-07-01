# TrackGit

Tracker Addon for Git Version Control System.

## About

This project is done under [Google Summer of Code 2018](https://summerofcode.withgoogle.com/organizations/4821756754264064/#4903524337451008). The mentor for this project is [Stephan Aßmus](https://github.com/stippi). The progress wiki is [here](https://github.com/Hrily/TrackGit/wiki) and the detailed progress reports can be found at [my Haiku blog](https://www.haiku-os.org/blog/hrily).

The project proposal can be found [here](https://drive.google.com/file/d/1nailoOCGmpRVo3sZ8mdXGmpuWxXdgf1T/view).

## Installation

Clone the repo and run following:

```
make
cp objects.x86-cc2-debug/TrackGit ~/config/non-packaged/add-ons/Tracker/
```

or you can execute `run.sh`.

## Features Added

+ [x] Init
+ [x] Status
+ [x] Clone
+ [x] Add Files
+ [x] Commit
+ [x] Pull
+ [ ] ShowConflicts
+ [ ] Push
+ [ ] CreateBranch
+ [ ] SwitchBranch
+ [ ] MergeBranch

## Documentation

The documentation is done using Doxygen. It can be found 
[here](https://hrily.github.io/TrackGit).
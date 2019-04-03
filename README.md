# Cole's Copy Program
A simple program for backing up files from an Apple computer. This program can be run from the desktop, as well as when booting from an install disk. This means that you can backup files from a computer that does not load into the OS, without the need to use of extra hardware. As well, it backups files regardless of error, whereas moving files in Finder will stop if one file errors.

## How to use
Step 1: Click on **Releases** and download the latest build.

### From the OS
Step 2: Just double-click the executable.

### From an installation disk/recovery disk/internet recovery
Step 2: Put the file onto a USB or some type of drive, then insert it into the computer in question.

Step 3: Load into your recovery media.

Step 4: Click **Utilities** > **Terminal**.

Step 5: Type in `/Volumes/**NAME_OF_THE_USB_WITH_THE_PROGRAM_AND_PATH**/run`

Then, just follow the CLI to backup/restore your computer.

> **NOTE:** The drive must be named **Macintosh HD** to back up. This might change in the future.

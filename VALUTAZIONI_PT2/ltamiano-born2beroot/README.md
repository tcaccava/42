*This project has been created as part of the 42 curriculum by ltamiano.*

# Born2beRoot

## Description

The objective of the Born2beRoot project to build a minimal Linux
server inside a virtual machine.

The machine is a Debian server installed on an encrypted LVM layout,
reachable only through SSH on a non-standard port. A monitoring script
broadcasts the state of the machine to every connected terminal at
regular intervals.

## Project description

### Choice of operating system

I chose **Debian** over Rocky Linux.

Debian's installer exposes manual partitioning with LUKS and LVM
directly. The package set is conservative and the defaults are already
close to what the subject asks for. Documentation and community
answers are abundant. Its disadvantages: packages are old by design,
which is a virtue on a server and a nuisance elsewhere, and Debian is
less representative of the RHEL-family conventions that dominate
corporate infrastructure.

Rocky Linux would have been the more "enterprise" answer — it is a
downstream rebuild of RHEL, so the knowledge transfers directly to a
large part of the industry, and SELinux is a genuinely stronger
mandatory access control system.  The cost is complexity. For a first
system administration project, Debian lets the effort go into
understanding the concepts rather than debugging the distribution.

### Design choices

**Partitioning.** A small unencrypted `/boot` partition holds the
kernel and initramfs. Everything else lives inside a single LUKS
container, and LVM sits *inside* that container so that one passphrase
unlocks one physical volume, and the logical volumes come along with
it. Separating `/var/log` and `/tmp` from `/` is the point of the
exercise: a runaway log file or a user filling `/tmp` cannot then fill
the root filesystem and take the system down. LVM makes the split safe
to commit to, because logical volumes can be resized later if a guess
turns out to be wrong.

**Security policies.** Password strength is enforced by
`pam_pwquality` and ageing by `login.defs` and `chage`. The two
mechanisms are independent: one governs what a password may be, the
other how long it may live.

**User management.** Beyond `root`, the user 'ltamiano' belongs to
`sudo`, which grants escalation, and to `user42`, which exists purely
as an evaluation requirement. Day-to-day work happens in that account;
`root` is never logged into directly and cannot be reached over SSH.

**Services installed.** Deliberately almost nothing: `openssh-server` on port
4242, `ufw` with a single allow rule, `cron` for the monitoring script, and
AppArmor, which Debian enables by default. 

### AppArmor vs SELinux

Both are Linux Security Modules implementing **mandatory access control**: they
constrain what a process may do regardless of Unix file permissions, so that a
compromised daemon running as root is still boxed in. They differ in how they
identify what they are protecting.

**AppArmor** is *path-based*. A profile names filesystem paths and the
operations allowed on them, in a syntax close to readable English. Profiles are
per-program, live in `/etc/apparmor.d/`, and a program with no profile is
simply unconfined.

**SELinux** is *label-based*. Every file, process, socket and port
carries a security context stored in extended attributes, and policy
is written in terms of those labels rather than paths. It is strictly
more powerful but the policy language is large, and diagnosing a
denial means reading audit logs and reasoning about contexts.

### UFW vs firewalld

Both are front-ends. Neither *is* the firewall — the enforcement
happens in the kernel's netfilter subsystem, reached through
`iptables` or `nftables`.

**UFW** ("Uncomplicated Firewall") is exactly what its name promises: a linear
list of rules, managed with commands like `ufw allow 4242`. 

**firewalld** organises rules into **zones** such as `public`, `home`,
`trusted` and so on and binds network interfaces to zones, so a laptop
moving between networks applies a different policy automatically. 

### VirtualBox vs UTM

**VirtualBox** is Oracle's cross-platform type-2 hypervisor. It runs on x86-64
hosts (Linux, Windows, macOS on Intel) and virtualises x86-64 guests using
hardware acceleration — VT-x on Intel, AMD-V on AMD. It is mature, its VDI disk
format and snapshot tooling are well understood, and it is the reference
platform for this project, which matters because the evaluation compares a hash
of the disk file.

**UTM** is a macOS front-end to QEMU. Its reason to exist here is Apple Silicon:
on an M-series Mac there is no x86-64 hardware virtualisation, so UTM either
virtualises ARM guests natively through Apple's Hypervisor framework, or
*emulates* x86-64 in software — correct but considerably slower. It stores disks
in QEMU's `qcow2` format.

The subject mandates VirtualBox and permits UTM only as a fallback when
VirtualBox cannot run. I used VirtualBox on an x86-64 Linux host.

## Resources

- [Debian OS](https://www.debian.org/)
- [AppArmor documentation](https://gitlab.com/apparmor/apparmor/-/wikis/Documentation)
- [UFW community documentation](https://help.ubuntu.com/community/UFW)
- [VirtualBox User Manual](https://www.virtualbox.org/manual/)

### Use of AI

AI assistance was used for the following, and only the following:

- Condensing the subject PDF into a structured checklist of requirements
  (`SPEC.org`), to make sure no mandatory item was missed.

- Drafting the build procedure in `INSTRUCTIONS.org`, which I then executed,
  debugged and corrected against the actual behaviour of the system.

- Creating the Makefile required to manage the project.

*This project has been created as part of the 42 curriculum by mde-matt.*

# Born2beRoot

## Description

Born2beRoot is a system administration project: the goal is to set up a minimal, secure Linux server inside a virtual machine, following strict rules on partitioning, users, password policy, sudo, SSH and firewall configuration, and to write a small monitoring script.

The server runs **Debian 13 (trixie)** in **VirtualBox**, without any graphical interface. Its main features are:

- encrypted partitions (LUKS) managed with LVM;
- an SSH service on port **4242**, with root login forbidden;
- the **UFW** firewall, with only port 4242 open;
- a strong password policy enforced through `login.defs`, `chage` and `pam_pwquality`;
- a strict `sudo` configuration, with full input/output logging;
- **AppArmor** active at startup;
- a `monitoring.sh` script that broadcasts system information to all terminals at startup and every 10 minutes.

The repository only contains this `README.md` and `signature.txt`, the SHA1 signature of the virtual machine's disk. The virtual machine itself is not included.

## Instructions

### Requirements

- VirtualBox (the VM disk is a `.vdi` file).
- The virtual disk `Born2beRoot.vdi`, stored on the campus machine in `/goinfre/mde-matt/Born2beRoot/`.

### Checking the signature

With the VM **powered off**, compare the disk's hash with the content of `signature.txt`:

```sh
sha1sum /goinfre/mde-matt/Born2beRoot/Born2beRoot.vdi
cat signature.txt
```

Starting the VM writes to the disk and changes its signature. For this reason, every evaluation starts without snapshots: a dedicated snapshot is created before booting, and at the end of the evaluation it is **restored first and then deleted**, so that the base disk is never modified.

### Starting the server

1. Start the VM from VirtualBox.
2. Enter the LUKS passphrase to unlock `sda5_crypt`.
3. Log in as `mde-matt` (or `root` from the console).

### Connecting through SSH

The VM uses VirtualBox's NAT network, so its address (`10.0.2.15`) is not reachable from the host. A port forwarding rule maps port 4242 of the host (bound to `127.0.0.1` only) to port 4242 of the VM. It can be created from the VM settings (Network → Adapter 1 → Port Forwarding) or, with the VM running:

```sh
VBoxManage controlvm Born2beRoot natpf1 "ssh,tcp,127.0.0.1,4242,,4242"
ssh mde-matt@localhost -p 4242
```

Connecting as root (`ssh root@localhost -p 4242`) is refused.

### Monitoring script

The script is located at `/usr/local/bin/monitoring.sh` and is run by root's crontab:

```
@reboot /usr/local/bin/monitoring.sh
*/10 * * * * /usr/local/bin/monitoring.sh
```

To stop it without modifying it, stop the cron service:

```sh
sudo systemctl stop cron     # sudo systemctl start cron to resume
```

## Project description

### Choice of operating system: Debian

Debian was chosen because the subject recommends it to people new to system administration, and because it is stable, extremely well documented and supported by a large community. Its package manager (`apt`/`dpkg`) is simple to use, and its default mandatory access control system, AppArmor, is much easier to work with than SELinux.

- **Advantages:** stability, documentation, large community, simple tooling, AppArmor enabled by default.
- **Disadvantages:** packages in the stable release are older than in other distributions, in exchange for stability; it is less common than Red Hat–based systems in some enterprise environments.

### Design choices

#### Partitioning

The disk (10 GB, MBR scheme) was partitioned with the installer's guided method *"use entire disk and set up encrypted LVM"*, with a separate `/home`:

```
sda                          10G  disk
├─sda1                      750M  part   /boot        (not encrypted)
├─sda2                        1K  part                (extended partition)
└─sda5                      9.3G  part                (logical, LUKS)
  └─sda5_crypt              9.2G  crypt               (LVM physical volume)
    ├─mde--matt42--vg-root    7G  lvm    /
    ├─mde--matt42--vg-swap_1 520M lvm    [SWAP]
    └─mde--matt42--vg-home  1.7G  lvm    /home
```

- `/boot` stays unencrypted because it contains the kernel and initramfs, which are needed to ask for the passphrase and decrypt the rest of the disk.
- MBR allows only four primary partitions, so the encrypted partition is a logical partition (`sda5`) inside an extended one (`sda2`).
- LVM sits on top of the encrypted volume: logical volumes can be resized or extended much more easily than classic partitions.
- The swap is inside the encrypted area, since memory pages written to it may contain sensitive data.
- Sizes: the root volume gets most of the space because it holds the system, the logs (`/var/log`) and future packages; `/home` stays small because the server has very few users; the total stays at 10 GB to avoid unnecessary disk usage.

#### Security policies

- **SSH** (`/etc/ssh/sshd_config`): `Port 4242` and `PermitRootLogin no`. Changing the default port reduces automated attacks; forbidding root forces attackers to guess a username as well, and every privileged action goes through `sudo`, where it is logged.
- **Firewall**: UFW with default policies *deny incoming, allow outgoing*, and a single rule allowing port 4242 (IPv4 and IPv6). It is enabled at boot.
- **Password policy**:
  - `/etc/login.defs`: `PASS_MAX_DAYS 30`, `PASS_MIN_DAYS 2`, `PASS_WARN_AGE 7` for new users;
  - `chage -M 30 -m 2 -W 7` applied to the existing users (`root`, `mde-matt`), because `login.defs` only applies at user creation;
  - `/etc/security/pwquality.conf` (via `libpam-pwquality`): `minlen = 10`, `ucredit = -1`, `lcredit = -1`, `dcredit = -1`, `maxrepeat = 3`, `usercheck = 1`, `difok = 7`, `enforce_for_root`;
  - all passwords were changed after the configuration, so that they comply with the policy.
- **sudo** (`/etc/sudoers.d/sudo_config`, edited with `visudo`):
  - `passwd_tries=3` and a custom `badpass_message`;
  - `logfile="/var/log/sudo/sudo.log"`, `log_input`, `log_output` and `iolog_dir="/var/log/sudo"`: every command and its inputs/outputs are recorded and can be replayed with `sudoreplay`;
  - `requiretty`: sudo can only be used from a real terminal;
  - `secure_path` restricts the directories where sudo looks for commands.
- **AppArmor** is enabled at startup and confines programs through profiles (checked with `aa-status`).

#### User management

- Hostname: `mde-matt42`.
- Besides `root`, the user `mde-matt` exists and belongs to the groups `sudo` and `user42`.
- New users are created with `adduser` and added to groups with `adduser <user> <group>` or `usermod -aG <group> <user>`.

#### Installed services

Only what the project requires was installed, starting from a minimal system (*standard system utilities* only, no desktop environment):

- `sudo`;
- `openssh-server`;
- `ufw`;
- `libpam-pwquality`;
- `cron` (part of the base system), used to run `monitoring.sh`.

#### monitoring.sh

A bash script that collects twelve pieces of information and broadcasts them with `wall`:

| Information | Source |
|---|---|
| Architecture and kernel | `uname -a` |
| Physical / virtual CPUs | `/proc/cpuinfo` (`physical id`, `processor`) |
| Memory usage | `free -m` |
| Disk usage | `df -m`, real disks only (`/dev/...`) |
| CPU load | `vmstat 1 2` (100 − idle time) |
| Last boot | `who -b` |
| LVM use | `lsblk` (any `lvm` volume) |
| TCP connections | `ss -t` (established connections) |
| Users logged in | `who` (distinct users) |
| IPv4 and MAC address | `hostname -I`, `ip link` |
| Commands run with sudo | `/var/log/sudo/sudo.log` (entries with a TSID, i.e. actually executed) |

### Comparisons

#### Debian vs Rocky Linux

| | Debian | Rocky Linux |
|---|---|---|
| Origin | community-driven | rebuild compatible with Red Hat Enterprise Linux |
| Target | general purpose, servers and desktops | enterprise servers |
| Packages | `apt` / `dpkg`, `.deb` | `dnf`, `.rpm` |
| Mandatory access control | AppArmor | SELinux |
| Default firewall tool | none by default (UFW in this project) | firewalld |
| Difficulty for this project | lower | higher (SELinux must be configured, e.g. to allow SSH on port 4242) |

#### AppArmor vs SELinux

Both are Mandatory Access Control (MAC) systems: they add rules on top of classic Unix permissions, which even processes running as root cannot bypass.

| | AppArmor | SELinux |
|---|---|---|
| Identifies resources by | file paths | security labels (contexts) on every file and process |
| Granularity | lower | very high |
| Configuration | readable per-program profiles, enforce or complain mode | complex policies, enforcing or permissive mode |
| Used by | Debian, Ubuntu | Rocky, RHEL, Fedora |

#### UFW vs firewalld

Both are front-ends to the kernel's packet filtering framework (netfilter, through nftables/iptables).

| | UFW | firewalld |
|---|---|---|
| Philosophy | simple rules (`ufw allow 4242`) | zones with different trust levels, services and ports assigned to zones |
| Changes | rules applied directly | runtime vs permanent configuration |
| Typical distributions | Debian, Ubuntu | Rocky, RHEL, Fedora |
| Learning curve | very gentle | steeper, more flexible |

#### VirtualBox vs UTM

Both are type 2 hypervisors (they run as applications on top of the host operating system).

| | VirtualBox | UTM |
|---|---|---|
| Developer | Oracle | open source, built on QEMU |
| Host systems | Windows, Linux, macOS | macOS (and iOS) |
| Strength | mature, cross-platform, widely documented | native virtualization on Apple Silicon, plus emulation of other architectures |
| Disk format | `.vdi` | `.qcow2` |
| Use in this project | used here, on the campus x86_64 machines | required on Apple Silicon Macs, where an arm64 guest is needed |

## Resources

### References

- [Debian installation guide (trixie)](https://www.debian.org/releases/trixie/installmanual)
- [The Debian Administrator's Handbook](https://debian-handbook.info/)
- [Debian Wiki: LVM](https://wiki.debian.org/LVM)
- [Debian Wiki: AppArmor](https://wiki.debian.org/AppArmor)
- [Ubuntu community documentation: UFW](https://help.ubuntu.com/community/UFW)
- Manual pages: `sshd_config(5)`, `sudoers(5)`, `visudo(8)`, `pwquality.conf(5)`, `login.defs(5)`, `chage(1)`, `ufw(8)`, `crontab(5)`, `wall(1)`, `lsblk(8)`, `ss(8)`, `awk(1)`

### Use of AI

AI (Claude) was used **mainly to explain concepts**, as a study companion.

All commands were run and verified by me on the virtual machine.

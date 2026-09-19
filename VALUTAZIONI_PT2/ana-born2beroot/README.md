*This project has been created as part of the 42 curriculum by anade-mo.*

# Born2beRoot

A Debian 13 server running in VirtualBox, set up by hand: encrypted disk with
LVM, strict `sudo` rules, a password policy, SSH on port 4242, a firewall, and
a monitoring script that broadcasts the machine's state to every terminal every
10 minutes. Bonus: the partition layout from the subject, a WordPress site
served by lighttpd + MariaDB + PHP, and Fail2ban.

| | |
|---|---|
| OS | Debian 13 (Trixie), no graphical interface |
| Hostname | `anade-mo42` |
| User | `anade-mo`, in the groups `sudo` and `user42` |
| SSH | port 4242, root login refused |
| Firewall | UFW: 4242 (SSH) and 80 (WordPress, bonus) |
| Monitoring | `/usr/local/bin/monitoring.sh`, run by root's cron every 10 min |

---

## Description

A Virtual Machine is a software simulation of a computer. They are run by a hypervisor, which can be either:
- Type 1: bare metal, direct hardware control
- Type 2: runs on host OS.

VirtualBox is a Type 2 hypervisor that was advised for this project because it is free, runs on Windows, macOS, Linux, and Solaris, and works well on x86 hardware.
For the OS, Debian 13 (Trixie) is a Linux distribution that was chosen because it is stable, has a huge community and documentation, and is beginner-friendly. 

The following rules and features were part of the project requirements:

Mandatory:

**System**
- Latest stable Debian (13, Trixie), **without any graphical interface**.
- **AppArmor** running at startup.
- **Hostname** = my login followed by 42: 'anade-mo42'

**Disk**
- At least 2 encrypted partitions using LVM.

**Users and groups**
- Besides root, a user named after my login: `anade-mo`.
- That user belongs to the **`user42`** and **`sudo`** groups.

**Password policy**
- Passwords expire every **30 days**, with a minimum of **2 days** between
  changes and a warning **7 days** before expiry.
- At least **10 characters**, with an uppercase letter, a lowercase letter and
  a number, and no more than **3 identical characters in a row**.
- The password must not contain the user's name.
- At least **7 characters different** from the previous password (this rule
  does not apply to root).
- The policy applies to root too, and **all passwords were changed** after the
  configuration was in place.

**sudo**
- Authentication limited to **3 attempts** on a wrong password.
- A **custom message** shown when the password is wrong.
- Every action logged, **inputs and outputs included**, in **`/var/log/sudo/`**.
- **TTY mode** enabled.
- The **paths** sudo may use are restricted (`secure_path`).

**Network**
- **SSH on port 4242** only, and connecting as **root over SSH is refused**.
- **UFW** active at startup, with **only port 4242 open** (the bonus adds 80).

**monitoring.sh**
- A bash script that broadcasts to **all terminals** at startup and **every 10
  minutes** (through `wall` and `cron`), with no errors displayed.
- It always shows: architecture and kernel version · number of physical
  processors · number of virtual processors · RAM available and its usage
  percentage · storage available and its usage percentage · CPU usage
  percentage · date and time of the last reboot · whether LVM is active ·
  number of active connections · number of users on the server · IPv4 and MAC
  addresses · number of commands run with sudo.
- It can be **interrupted without modifying the file** (by commenting out the
  cron job or stopping the cron service).

## Instructions

The repository holds only `README.md` and `signature.txt`: the SHA-1
fingerprint of the virtual disk (`.vdi`). The machine itself is not submitted on git.

To evaluate:

1. Start the VM in VirtualBox and enter the LUKS passphrase at boot.
2. Log in as `anade-mo` (never as root; root has no SSH access).
3. From the host, SSH in: `ssh anade-mo@localhost -p 4242`
   (VirtualBox forwards host port 4242 to the VM's 4242).
4. Bonus site: `http://localhost:8080` (host 8080 → VM port 80).

### Useful checks

One block per requirement, each meant to be copied on its own.

**System: version, no graphical interface, AppArmor, hostname**
```bash
cat /etc/os-release | head -2
ls /usr/bin | grep -iE "xorg|wayland"      # prints nothing
systemctl is-active apparmor
hostname
```

**Disk: encrypted LVM and the mounted volumes**
```bash
lsblk
sudo vgs
sudo lvs
df -h | grep mapper
```

**Disk: give a volume more space (what LVM is for)**
```bash
sudo vgs                                   # VFree = space still unused in LVMGroup
sudo lvextend -L +1G /dev/LVMGroup/home    # add 1 GB to the volume
sudo resize2fs /dev/LVMGroup/home          # let the filesystem use it
df -h /home                                # the new size
```
`lvextend` enlarges the logical volume, `resize2fs` enlarges the ext4
filesystem inside it. Both work while the system is running, as long as the
volume is growing and `vgs` shows free space. Shrinking is the opposite case:
the volume has to be unmounted first.

**Users and groups**
```bash
getent group sudo user42
getent passwd anade-mo
```

**Password policy: aging and strength**
```bash
sudo chage -l anade-mo
sudo chage -l root
grep -E "^PASS_(MAX|MIN|WARN)" /etc/login.defs
grep pam_pwquality /etc/pam.d/common-password
```

**sudo: rules and logs**
```bash
sudo cat /etc/sudoers.d/born2beroot
sudo ls -l /var/log/sudo/
sudo tail -3 /var/log/sudo/sudo.log
```

**Network: SSH and firewall**
```bash
sudo grep -E "^Port|PermitRootLogin" /etc/ssh/sshd_config
sudo ss -tlnp | grep 4242
sudo ufw status numbered
```

**Monitoring script**
```bash
sudo crontab -l
ls -l /usr/local/bin/monitoring.sh
cat /usr/local/bin/monitoring.sh
```

**Bonus: WordPress and Fail2ban**
```bash
systemctl is-active lighttpd mariadb fail2ban
dpkg -l | grep -E "apache2|nginx"          # prints nothing
sudo mariadb -e "SHOW DATABASES;"
sudo fail2ban-client status sshd
```

**Everything at once** (one paste, each section labelled)
```bash
echo "== system" && cat /etc/os-release | head -2 && systemctl is-active apparmor && hostname
echo "== disk" && lsblk
echo "== users" && getent group sudo user42
echo "== password" && sudo chage -l anade-mo | head -5 && grep pam_pwquality /etc/pam.d/common-password
echo "== sudo" && sudo cat /etc/sudoers.d/born2beroot
echo "== network" && sudo grep -E "^Port|PermitRootLogin" /etc/ssh/sshd_config && sudo ufw status
echo "== monitoring" && sudo crontab -l | grep monitoring
echo "== bonus" && systemctl is-active lighttpd mariadb fail2ban
```
`&&` runs the next command only if the previous one succeeded, so a failure
stops that line and shows where the problem is.

To run the monitoring script immediately: `sudo bash /usr/local/bin/monitoring.sh`
(from an SSH session this prints twice, because `sudo` runs it in its own
terminal; a cron run shows it once).

## Project description

### Why Debian

Debian was recommended for beginners in the subject, and it fits this project:

**Advantages**
- Stable: packages change slowly.
- Huge documentation and community.
- `apt` is a simple package manager for installing, updating software packages.
- AppArmor is a security module restricting program capabilities via access control. It is installed and active by default.
- Very light: a fresh install idles at a few hundred MB of RAM.

**Disadvantages**
- Packages are older than on rolling-release distributions, so the newest
  software versions need extra repositories.
- Not the enterprise standard: companies using RHEL-compatible systems expect
  Rocky's tooling (`dnf`, SELinux) instead.
- Defaults are minimal: much has to be configured by hand, slowing down the setup.

### Design choices

**LVW - Logical Volume Manager** Partitioning (bonus layout): One 500 MB plain `/boot`, everything else in an encrypted partition holding the LVM volume group `LVMGroup`:
| Volume | Mount | Size | Why |
|---|---|---|---|
| `root` | `/` | 9.3 GB | the system; must never fill up |
| `swap` | swap | 2.2 GB | when RAM overflow it uses swap, about the size of the VM's RAM |
| `home` | `/home` | 9.7 GB | user files, kept away from the system |
| `var` | `/var` | 2.8 GB | variable data, such as WordPress (`/var/www`) and MariaDB (`/var/lib/mysql`) grow here |
| `srv` | `/srv` | 2.8 GB | data served by the machine, websites and other services |
| `tmp` | `/tmp` | 2.8 GB | stores temporary files, non critical, cleared periodically |
| `var-log` | `/var/log` | 3.7 GB | stores logs from the system for troubleshooting |

`/boot` stays unencrypted because the bootloader has to read the kernel before
anyone can type the passphrase.

**Security policies.**
- `sudo` rules (`/etc/sudoers.d/born2beroot`): 3 password attempts, a custom
  message on failure, logs of commands and their input/output in
  `/var/log/sudo/`, TTY required, and a restricted `secure_path`.
- Password aging in `/etc/login.defs` and applied with `chage`: expires after
  30 days, at least 2 days between changes, 7 days of warning.
- Password strength through PAM's `pwquality`: at least 10 characters, one
  uppercase, one lowercase, one digit, no more than 3 identical characters in a
  row, cannot contain the username, at least 7 characters different from the
  previous password, and the rules apply to root too.
- SSH: port 4242 instead of 22, and root login disabled.
- UFW: deny everything inbound, allow 4242 and (bonus) 80.
- AppArmor: active, confining programs to their profiles.

**User management.** `anade-mo` is in `sudo` (admin rights through logged,
per-command elevation) and in `user42` (required by the subject). Root is only
reachable through `sudo` or the VM console, never over SSH.

**Services installed.**
- `openssh-server` — remote access on 4242.
- `ufw` — firewall.
- `cron` — runs `monitoring.sh` every 10 minutes.
- Bonus: `lighttpd`, `mariadb-server` and `php-cgi` for WordPress;
  `fail2ban` as the extra service.

**The monitoring script.** `monitoring.sh` collects each value with one small
command (`uname`, `free`, `df`, `top`, `who`, `lsblk`, `ss`, `hostname`, `ip`),
builds one message, and broadcasts it with `wall`. On Debian 13 `wall` reaches
the local console but not SSH sessions, because logged-in SSH sessions are
recorded as `sshd` rather than as their terminal, so the script also writes the
message to each `/dev/pts/*` terminal. The sudo counter reads
`/var/log/sudo/sudo.log`, the log configured in the sudo rules above, so the
count survives reboots.

### Bonus

- **Partitions** following the subject's layout (table above).
- **WordPress** served by lighttpd with `php-cgi` over FastCGI and MariaDB for
  storage, reachable on port 80. Content: my portfolio, imported with WP-CLI 
  (WordPress Command Line Interface) from a local backup of my existing site.
- **Fail2ban** as add-on service of my choice. It watches the systemd journal for
  failed SSH logins and bans an address for 10 minutes after 3 failures within
  10 minutes, adding the ban as a UFW rule. It was chosen because SSH is the
  only way into this machine: the password policy makes guessing slow and UFW
  decides which ports are open, but nothing otherwise limits how many times an
  address may try. It adds no open port and almost no load.

### Comparisons

**Debian vs Rocky Linux.** Both are free server distributions. Debian is
community-run, uses `.deb` packages with `apt`, ships AppArmor, and releases on
its own schedule. Rocky is a rebuild of Red Hat Enterprise Linux: `.rpm`
packages with `dnf`, SELinux, and a long support cycle aimed at enterprises.
Debian is the gentler starting point and is what the subject recommends for
beginners; Rocky is what a company standardised on RHEL would use.

**AppArmor vs SELinux.** Both add Mandatory Access Control: they restrict what
a program may do, beyond normal file permissions, so a compromised program can
do less damage. AppArmor works with **file paths** and its profiles are short
and readable; it is Debian's default. SELinux works with **labels** attached to
files and processes, which is more precise and more powerful, but harder to
write and to debug; it is the default on Rocky.

**UFW vs firewalld.** Both are front-ends to the kernel's own firewall. UFW is
rule-based and deliberately simple (`ufw allow 4242`), which suits a server
with two open ports. firewalld is zone-based: interfaces belong to zones with
different trust levels, and rules can change without reloading everything,
which suits machines with several networks. UFW is Debian's usual choice,
firewalld is RHEL's.

**VirtualBox vs UTM.** Both run virtual machines on a desktop computer.
VirtualBox is free, cross-platform, and works well on x86 hardware, which is
why the subject mandates it; it is what I used on Windows. UTM is a front-end
to QEMU used mainly on Apple-Silicon Macs, where VirtualBox does not run
properly; it is the subject's fallback for those machines.

## Resources

- 42's alumni documentation websites, youtube videos and github repositories. https://born2beroot-tracking.pages.dev/; https://noreply.gitbook.io/born2beroot/; 
- Debian Administrator's Handbook and `man` pages: `sudoers`, `login.defs`, `sshd_config`, `ufw`, `crontab`, `wall`, `lvm`.
- WordPress and WP-CLI documentation for the bonus.
- Youtube videos on VM, Linux kernel, SSH.
- AI (Anthropic) was used for: organizing milestones and planning; checking all requirements were satisfied; solving unexpected behavior of my PC compatibility with VirtualBox, Windows and Windows Subsystem for Linux. providing the required commands for the checks; debugging why `wall` did not reach SSH sessions on Debian 13; reviewing syntax on `monitoring.sh`and converting existant films.json into WordPress posts. readme formatting and grammar.

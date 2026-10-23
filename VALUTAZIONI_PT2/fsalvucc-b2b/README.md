*This project has been created as part of the 42 curriculum by fsalvucc.*

# Born2beRoot

## Description

Born2beRoot is a system administration project: install and harden a Linux server in a
virtual machine, with strict rules on partitioning, users, passwords, privileges, SSH
and firewall, and no graphical interface.

This VM runs **Rocky Linux 9** on **VirtualBox** and covers:

- **Mandatory part**: encrypted LVM, strong password policy, strict `sudo` with full
  logging, SSH on port 4242 without root login, `firewalld`, SELinux enforcing, and a
  `monitoring.sh` script broadcast with `wall` every 10 minutes.
- **Bonus part**: the bonus partition layout, WordPress on **lighttpd + MariaDB +
  PHP-FPM**, and **Monit** supervising `sshd`, `lighttpd`, `php-fpm` and `mariadb`.

## Instructions

1. Check the disk signature (run the evaluation on a clone, booting changes the hash):
   ```bash
   sha1sum Born2beRoot.vdi    # must match signature.txt
   ```
2. VirtualBox NAT port forwarding: host `127.0.0.1:4242` → guest `4242` (SSH),
   host `127.0.0.1:8080` → guest `80` (WordPress).
3. Boot the VM, enter the **LUKS passphrase**, then log in:
   ```bash
   ssh -p 4242 fsalvucc@127.0.0.1
   ```
4. Bonus: WordPress at `http://127.0.0.1:8080`; Monit status with `sudo monit summary`
   (web UI on localhost only, reachable with
   `ssh -L 2812:localhost:2812 -p 4242 fsalvucc@127.0.0.1`).

## Project description

### Why Rocky Linux

Rocky is a free rebuild of Red Hat Enterprise Linux.

- **Pros**: enterprise standard, ~10 years of support, SELinux enforcing by default.
- **Cons**: harder after installation (SELinux, `authselect`, `firewalld`), smaller
  base repositories (lighttpd and Monit need EPEL), fewer beginner tutorials.

### Main design choices

- **Partitioning**: `/boot` unencrypted; one LUKS partition with a single LVM PV and
  VG `LVMGroup` (`root`, `swap`, `home`, `var`, `srv`, `tmp`, `var-log`). XFS cannot
  shrink, so free space was left in the VG to grow volumes later.
- **Password policy**: 30/2/7 days expiration (`login.defs`, `chage` for root);
  `pwquality.conf` with min 10 chars, upper, lower and digit, max 3 repeated chars,
  no username, 7 chars different from the old one, enforced for root.
- **sudo** (`/etc/sudoers.d/`, edited with `visudo`): 3 tries, custom error message,
  event and I/O logs in `/var/log/sudo/`, `requiretty`, restricted `secure_path`.
- **Users**: `fsalvucc` in `user42` and `sudo` (a dedicated `sudo` group instead of
  `wheel`), created after the policy so it is compliant from the start.
- **Security**: SSH on 4242 (labelled `ssh_port_t` in SELinux), root login disabled;
  firewalld allows only 4242 and 80; SELinux always enforcing.
- **Services**: `monitoring.sh` via root's cron. Bonus: MariaDB on Unix socket only
  with a least-privilege user, PHP-FPM as `lighttpd`, WordPress in `/srv/wordpress`
  with code read-only for PHP and no PHP execution in `uploads`; Monit checks each
  service by connecting to it and restarts it through `systemctl`.

### Debian vs Rocky Linux

| | Debian | Rocky Linux |
|---|---|---|
| Base | independent community distro | RHEL rebuild |
| Packages | `apt` / `.deb` | `dnf` / `.rpm` |
| Security module | AppArmor | SELinux |
| Firewall | UFW (commonly added) | firewalld |
| Difficulty | easier, more tutorials | harder after installation |

### AppArmor vs SELinux

| | AppArmor | SELinux |
|---|---|---|
| Model | path-based profiles per program | labels on every process, file and port |
| Complexity | simpler | more complex, more granular |
| Default on | Debian, Ubuntu | RHEL, Rocky, Fedora |

### UFW vs firewalld

| | UFW | firewalld |
|---|---|---|
| Model | simple allow/deny rules | zones and services |
| Changes | applied and saved at once | separate runtime and permanent config |
| Default on | Ubuntu | RHEL, Rocky, Fedora |

### VirtualBox vs UTM

| | VirtualBox | UTM |
|---|---|---|
| Hosts | Linux, Windows, macOS | macOS and iOS only |
| Engine | own hypervisor | Apple Hypervisor / QEMU |
| Best for | cross-platform, 42 machines | Apple Silicon Macs |

## Resources

- [Rocky Linux documentation](https://docs.rockylinux.org/)
- [RHEL 9 – Using SELinux](https://docs.redhat.com/en/documentation/red_hat_enterprise_linux/9/html/using_selinux/index)
- [firewalld documentation](https://firewalld.org/documentation/)
- [EPEL documentation](https://docs.fedoraproject.org/en-US/epel/)
- [lighttpd wiki](https://redmine.lighttpd.net/projects/lighttpd/wiki)
- [WordPress – Hardening](https://developer.wordpress.org/advanced-administration/security/hardening/)
- [Monit manual](https://mmonit.com/monit/documentation/monit.html)
- Man pages: `sudoers(5)`, `pwquality.conf(5)`, `sshd_config(5)`, `crontab(5)`, `semanage(8)`

### Use of AI

Claude (Anthropic) was used as a tutor, mainly for the **bonus part**: explaining the
lighttpd/PHP-FPM/MariaDB setup, SELinux labels and denials, and Monit configuration,
and helping with troubleshooting. It also helped draft this README. Every change was made, tested and verified by me.

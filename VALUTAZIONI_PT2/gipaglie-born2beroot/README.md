*This project has been created as part of the 42 curriculum by gipaglie*

## Description

Born2beRoot is an introduction to virtualization and system administration. The objective is to create a virtual machine that functions as a server, adhering to strict security rules and providing only the minimum required services. This project emphasizes hands-on experience with server setup, security configuration, and system administration concepts rather than programming.

The project involves:
- Setting up a virtual machine using VirtualBox
- Configuring encrypted partitions with LVM
- Implementing strong security policies
- Configuring SSH for remote access
- Setting up a firewall
- Creating an automated system monitoring script

## Instructions

### Prerequisites
- VirtualBox installed on your host machine
- Debian ISO (latest stable version)
- At least 30GB of free disk space

### Installation Steps

1. **Create Virtual Machine**
   - Open VirtualBox and create a new machine
   - Allocate sufficient RAM (recommended: 4GB+)
   - Create a virtual hard disk (30GB, dynamically allocated)

2. **Install Debian**
   - Boot from Debian ISO
   - Select "Install" (graphical interface is forbidden)
   - Choose manual partitioning to create LVM encrypted partitions
   - Create at least 2 encrypted partitions as shown in the subject

3. **System Configuration**
   - Set hostname to `<login>42` (e.g., `fcorvaro42`)
   - Create user account with your login name
   - Install and configure sudo with strict policies
   - Configure SSH on port 4242 (root login forbidden)
   - Set up UFW firewall (only port 4242 open)
   - Implement strong password policy
   - Configure AppArmor to run at startup

4. **Monitoring Script**
   - Create `/home/<username>/monitoring.sh`
   - Configure crontab to run script every 10 minutes: `*/10 * * * * sh /home/<username>/monitoring.sh`

### Verification Commands

```bash
# Check SSH is running on port 4242
sudo ss -tunlp | grep 4242

# Verify UFW firewall status
sudo ufw status

# Test password policy requirements
sudo passwd <username>

# Check sudo configuration
sudo visudo -c

# Verify AppArmor status
sudo aa-status

# Run monitoring script manually
sh /home/<username>/monitoring.sh
```

## Project Description

### Choice of Operating System: Debian

**Debian was chosen for this project** because:

#### Advantages
- **Stability**: Debian prioritizes stability over having the latest features
- **Large Community**: Extensive documentation and community support
- **Package Management**: APT package manager is intuitive and well-documented
- **Learning Curve**: Debian is ideal for beginners in system administration
- **Security**: Strong focus on security updates and patches
- **Simplicity**: Straightforward configuration compared to more complex distributions

#### Disadvantages
- **Outdated Packages**: Sometimes packages are older than in rolling-release distributions
- **Less Cutting-Edge**: Not ideal for users who need the latest software versions

### Design Choices

#### 1. **LVM Encryption (Partitioning)**
LVM (Logical Volume Manager) was configured with encrypted partitions to:
- Protect sensitive data at rest
- Provide flexibility in partition management
- Ensure secure boot process with passphrase protection
- Allow logical resizing of partitions if needed

#### 2. **SSH Security (Port 4242)**
- Changed from default port 22 to 4242 to reduce attack surface
- Disabled root login via SSH to prevent direct privilege escalation
- Requires authentication with username and password

#### 3. **Password Policy**
Implemented strong password requirements:
- 30-day expiration
- Minimum 10 characters with uppercase, lowercase, and numbers
- Maximum 3 consecutive identical characters
- Cannot contain username
- 7 characters different from previous password (root only)

#### 4. **Sudo Configuration**
Configured with security constraints:
- Limited to 3 password attempts
- All commands logged to `/var/log/sudo/`
- TTY mode enabled for security
- Restricted PATH for security

#### 5. **Firewall (UFW)**
UFW provides simple firewall management:
- Only port 4242 (SSH) is open
- All other incoming traffic is blocked
- Enabled by default on system startup

#### 6. **AppArmor (Mandatory Confinement)**
AppArmor restricts program capabilities:
- Runs at startup
- Provides additional security layer beyond traditional permissions
- Prevents unauthorized access to system resources

### Comparisons

#### **Debian vs Rocky Linux**

| Aspect | Debian | Rocky Linux |
|--------|--------|-------------|
| **Base** | Independent | RHEL-based |
| **Package Manager** | APT | YUM/DNF |
| **Release Cycle** | 2-3 years for major releases | Following RHEL schedule |
| **Learning Curve** | Easier for beginners | Steeper learning curve |
| **Use Case** | Desktop, server, embedded | Enterprise server |
| **Community** | Large, very active | Growing, enterprise-focused |

**Why Debian**: Debian is more suitable for this project because of its beginner-friendly approach and extensive documentation.

#### **AppArmor vs SELinux**

| Aspect | AppArmor | SELinux |
|--------|----------|---------|
| **Complexity** | Simpler, path-based | Complex, label-based |
| **Configuration** | Easier to learn | Steeper learning curve |
| **Performance** | Lighter overhead | More resource-intensive |
| **Flexibility** | Less granular control | More granular control |
| **Default State** | Permissive | Can be very restrictive |

**Why AppArmor for Debian**: AppArmor is the default in Debian, is easier to configure, and provides sufficient security for most use cases.

#### **UFW vs firewalld**

| Aspect | UFW | firewalld |
|--------|-----|-----------|
| **Backend** | iptables | nftables/iptables |
| **Complexity** | Very simple commands | More complex |
| **Learning** | Beginner-friendly | Requires more knowledge |
| **Dynamic Rules** | Less dynamic | Highly dynamic |
| **Default in Debian** | Not installed by default | Not installed by default |
| **Use Case** | Simple firewalls | Complex enterprise firewalls |

**Why UFW for Debian**: UFW is easier to learn and understand, making it perfect for a security education project.

#### **VirtualBox vs UTM**

| Aspect | VirtualBox | UTM |
|--------|-----------|-----|
| **Supported OS** | Windows, Mac, Linux | macOS only |
| **Performance** | Good | Excellent on Apple Silicon |
| **Ease of Use** | Very intuitive | User-friendly |
| **Price** | Free, Open Source | Free, Open Source |
| **Platform** | Cross-platform | Apple Silicon/Intel Macs |

**Why VirtualBox**: VirtualBox works on all major platforms and is the recommended choice for this project.

## Resources

### Official Documentation
- [Debian Official Website](https://www.debian.org/)
- [Debian Installation Guide](https://www.debian.org/releases/stable/installmanual/)
- [Ubuntu/Debian SSH Documentation](https://help.ubuntu.com/community/SSH)
- [UFW - Uncomplicated Firewall](https://help.ubuntu.com/community/UFW)
- [AppArmor Documentation](https://gitlab.com/apparmor/apparmor/-/wikis/home)
- [LVM - Linux Logical Volume Manager](https://tldp.org/HOWTO/LVM-HOWTO/)
- [Linux PAM - Password Policy](http://www.linux-pam.org/)

### Security Resources
- [OWASP - Authentication Cheat Sheet](https://cheatsheetseries.owasp.org/cheatsheets/Authentication_Cheat_Sheet.html)
- [Linux Security Hardening Guide](https://wiki.archlinux.org/title/Security)
- [SSH Security Best Practices](https://linux-audit.com/ssh-security-best-practices/)

### Tutorials & Articles
- [How to Configure Sudo on Linux](https://www.cyberciti.biz/faq/linux-unix-running-sudo-command-without-password/)
- [Understanding Linux Partitioning](https://www.linux.com/training-tutorials/partitioning-linux-hard-disk-lvm/)
- [Bash Scripting Guide](https://www.gnu.org/software/bash/manual/bash.html)
- [Cron Job Tutorial](https://opensource.com/article/17/11/how-use-cron-linux)

## AI Usage

### Which Tasks Used AI
1. **Syntax validation for bash script** - Used AI to verify script correctness
2. **Password policy configuration** - AI provided examples of PAM configuration
3. **Documentation structure** - AI helped structure this README

### Which Tasks Did NOT Use AI
- All system configuration and setup
- Manual partitioning and LVM configuration
- SSH and firewall configuration
- Script logic and understanding
- Security policy decisions

### Specific AI Contributions
- Provided correct awk/sed command syntax for system info extraction
- Suggested efficient bash commands for monitoring script
- Validated regular expressions and string manipulation

---

**Note**: This project was completed following the 42 curriculum's emphasis on learning fundamentals without shortcuts. All configurations were manually tested and verified.

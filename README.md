# BullFrog OS

<p align="center">
  <img width="501" height="458" alt="BullFrog OS" src="https://github.com/user-attachments/assets/4995fcf8-d87c-42c4-ac91-38a86c8d52c4" />
</p>

### Your Linux. Your environment. Your rules.

BullFrog OS is a custom Debian-based Linux distribution featuring XFCE, Firefox ESR, and our own BullFrog welcome experience.

## Version 0.1.0

- **Base:** Debian 13 (Trixie)
- **Architecture:** AMD64 (x86-64)
- **Desktop:** XFCE
- **Display manager:** LightDM
- **Browser:** Firefox ESR
- **Boot:** Live ISO, tested with UEFI

## Features

- Debian-based Linux system
- XFCE desktop environment
- Firefox ESR web browser
- XFCE Terminal
- NetworkManager
- Garry the Frog mascot
- Custom BullFrog welcome dialog

## System Requirements

BullFrog OS 0.1.0 is designed for compatible 64-bit Intel and AMD computers.

### Minimum Requirements (Provisional)

- **Processor:** 64-bit x86-64 (AMD64) compatible CPU
- **RAM:** 2 GB
- **Storage:** 8 GB of available storage for an installation
- **Graphics:** Graphics hardware supported by the Linux kernel and XFCE
- **Boot mode:** UEFI
- **Bootable USB:** 4 GB or larger
- **Display:** 1024 × 768 resolution recommended

### Recommended Requirements

- **Processor:** Dual-core or better 64-bit CPU
- **RAM:** 4 GB or more
- **Storage:** 20 GB or more
- **Graphics:** Hardware with compatible Linux graphics drivers
- **Boot mode:** UEFI
- **Internet:** Network connection for web browsing and downloading updates

### Hardware Compatibility Notes

- These requirements are preliminary estimates and have not been verified on every hardware configuration.
- BullFrog OS 0.1.0 has been tested in a virtual machine using UEFI. Physical computer compatibility may vary.
- Wi-Fi, Bluetooth, audio, graphics acceleration, and other hardware features may require additional drivers or firmware.
- Legacy BIOS booting is not currently confirmed.
- Test the live system before installation to check hardware compatibility.
- Back up important data before installing any operating system.

## Download

Visit the [BullFrog OS GitHub repository](https://github.com/CodeAnd-Copper/BullFrog-OS) to find the source code and any published release assets.

## Boot BullFrog OS Manually from GRUB

If the GRUB menu does not appear, you may be able to boot the live system manually.

### 1. Find the ISO device

At the `grub>` prompt, enter:

```grub
ls
```

Inspect the available devices. The ISO may appear as `(cd0)`, `(hd0)`, or another device.

### 2. Find the kernel and initrd

For example:

```grub
ls (cd0)/live/
```

Look for the kernel file, commonly named `vmlinuz`, and the initial RAM filesystem, commonly named `initrd.img`.

**Use the actual filenames and paths displayed by your system.** The examples below assume both files are in `/live/` on `(cd0)`.

### 3. Load the Linux kernel

```grub
linux /live/vmlinuz boot=live
```

### 4. Load the initial RAM filesystem

```grub
initrd /boot/initrd.img
```

### 5. Boot BullFrog OS

```grub
boot
```

If the files are on a different device, replace `(cd0)` with the correct device. If the kernel or initrd has a different name or location, adjust the paths accordingly.

These are general Debian Live GRUB commands. The exact paths and boot parameters must be verified against the particular BullFrog OS ISO. If the commands fail, inspect the device contents and confirm the correct paths before trying other commands.

## Built With

- Debian GNU/Linux
- Debian Live Build
- XFCE
- Python and Zenity

## Creator

Made by [CodeAndCopper](https://github.com/CodeAnd-Copper).

BullFrog OS is an independent project focused on Linux customization, experimentation, and learning.

## Feedback

Found a bug? Please open an issue in the GitHub repository and include:

- Your computer or virtual machine configuration
- Whether you used UEFI or BIOS
- The exact error message
- The steps needed to reproduce the issue

## Support BullFrog OS ☕

Enjoying BullFrog OS? You can support the project's development here:

**[Buy me a coffee](https://buymeacoffee.com/CodeAndCopper)**

Every bit of support is appreciated and helps motivate future improvements to BullFrog OS. 🐸

## License

MIT

---

**BullFrog OS 0.1.0**

*Your Linux. Your environment. Your rules.*

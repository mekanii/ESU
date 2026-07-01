# CHANGELOG
Semua perubahan penting pada proyek **ESU** akan dicatat di file ini.
Format ini berbasis pada [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## 2026-06-25
### Added
-   File PCB `hardware/REM-7.0`, reverse engineering dari REM board `TAKTVOLL v1.0 230927`
    - Versi sebelumnya terkendala pada stabilitas pembacaan saat trigger monopolar aktif di power tinggi (EMI effect).
    - Versi ini menerapkan komponen SMD pada sisi primer, desain double layer dengan top-bottom ground plane serta ruang soldering untuk can-style copper plate sebagai Faraday Cage.
-   File .pdf .dxf `hardware/REM-7.0`, untuk membuat trace pada copper board menggunakan laser.

## 2026-01-01
-   Firmware controller `firmware/ESU_V1`

## 2026-01-01
### Added
-   PCB Design file `hardware/PS-`
-   PCB Design file `hardware/MSD-`
-   PCB Design file `hardware/REM-`
-   PCB Design file `hardware/CTL-`
-   Firmware display `ui`
-   Firmware controller `firmware/ESP32-S3 PWM`

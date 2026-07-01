# CHANGELOG
All notable changes to this **ESU** project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/2.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.3] 2026-06-25
### Stack
-   Hardware
    - PS-5
    - MSD
    - REM
    - CTL-4
-   Firmware `ESU_V1`
-   UI

### Added
-   File PCB `hardware/REM-7.0`, reverse engineering dari REM board `TAKTVOLL v1.0 230927`
    - Versi sebelumnya terkendala pada stabilitas pembacaan saat trigger monopolar aktif di power tinggi (EMI effect).
    - Versi ini menerapkan komponen SMD pada sisi primer, desain double layer dengan top-bottom ground plane serta ruang soldering untuk can-style copper plate sebagai Faraday Cage.
-   File .pdf .dxf `hardware/REM-7.0`, untuk membuat trace pada copper board menggunakan laser.

## [1.0.1] 2026-01-01
### Added
-   Firmware controller `firmware/ESU_V1`

## [1.0.0] 2026-01-01
### Added
-   PCB Design file `hardware/PS-`
-   PCB Design file `hardware/MSD-`
-   PCB Design file `hardware/REM-`
-   PCB Design file `hardware/CTL-`
-   Firmware display `ui`
-   Firmware controller `firmware/ESP32-S3 PWM`

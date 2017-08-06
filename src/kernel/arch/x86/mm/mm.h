#pragma once

// Page Directory Flags

enum PageDirectoryFlag {
    kPageDirectoryFlagPresent           = 1,
    kPageDirectoryFlagReadWrite         = 1 << 1,
    kPageDirectoryFlagUserSupervisor    = 1 << 2,
    kPageDirectoryFlagWriteThrough      = 1 << 3,
    kPageDirectoryFlagCacheDisabled     = 1 << 4,
    kPageDirectoryFlagAccessed          = 1 << 5,
    kPageDirectoryFlagZero              = 1 << 6,
    kPageDirectoryFlagPageSize          = 1 << 7,
    kPageDirectoryFlagIgnored           = 1 << 8
}

enum PageTableFlag {
    kPageTableFlagPresent           = 1,
    kPageTableFlagReadWrite         = 1 << 1,
    kPageTableFlagUserSupervisor    = 1 << 2,
    kPageTableFlagWriteThrough      = 1 << 3,
    kPageTableFlagCacheDisabled     = 1 << 4,
    kPageTableFlagAccessed          = 1 << 5,
    kPageTableFlagDirty             = 1 << 6,
    kPageTableFlagZero              = 1 << 7,
    kPageTableFlagGlobal            = 1 << 8
}
#ifndef CONSTANT_H
#define CONSTANT_H

static const char ERROR_OPENING_FILE_FOR_WRITING[] = "Could not open %s for writting.\n";
static const char ERROR_OPENING_FILE_FOR_READING[] = "Could not open %s for reading.\n";
static const char ERROR_NO_SPACE_AVAILABLE[] = "No more space in tree.\n";
static const char ERROR_CREATING_THREAD[] = "Error when creating thread.\n";
static const char WRITE_BINARY[] = "wb";
static const char READ_BINARY[] = "rb";
static const char APPEND_BINARY[] = "ab";
static const char LSM_FILENAME_PREFIX[] = "lsm-level-";
static const char FILE_EXTENSION[] = ".bin";

#endif /* CONSTANT_H */
#include "NoiseStreamerException.h"

const DomainErrorCode GNR0001 = DomainErrorCode("GNR0001", "Cannot open file '%s'");
const DomainErrorCode GNR0002 = DomainErrorCode("GNR0002", "Invalid file line number '%s'");

const DomainErrorCode ARG0001 = DomainErrorCode("ARG0001", "Missing Argument '%s'");

const DomainErrorCode ENC0001 = DomainErrorCode("ENC0001", "Lame parameters failed to initialize properly");

const DomainErrorCode NSS0001 = DomainErrorCode("NSS0001", "Could not allocate shout_t");
const DomainErrorCode NSS0002 = DomainErrorCode("NSS0002", "Error setting hostname: '%s'");
const DomainErrorCode NSS0003 = DomainErrorCode("NSS0003", "Error setting port: '%s'");
const DomainErrorCode NSS0004 = DomainErrorCode("NSS0004", "Error setting agent: '%s'");
const DomainErrorCode NSS0005 = DomainErrorCode("NSS0005", "Error setting username: '%s'");
const DomainErrorCode NSS0006 = DomainErrorCode("NSS0006", "Error setting password: '%s'");
const DomainErrorCode NSS0007 = DomainErrorCode("NSS0007", "Error setting mountpoint: '%s'");
const DomainErrorCode NSS0008 = DomainErrorCode("NSS0008", "Error setting name: '%s'");
const DomainErrorCode NSS0009 = DomainErrorCode("NSS0009", "Error setting url: '%s'");
const DomainErrorCode NSS0010 = DomainErrorCode("NSS0010", "Error setting genre: '%s'");
const DomainErrorCode NSS0011 = DomainErrorCode("NSS0011", "Error setting description: '%s'");
const DomainErrorCode NSS0012 = DomainErrorCode("NSS0012", "Error setting dumpfile: '%s'");
const DomainErrorCode NSS0013 = DomainErrorCode("NSS0013", "Error setting audio data: '%s'");
const DomainErrorCode NSS0014 = DomainErrorCode("NSS0014", "Error setting metadata: '%s'");
const DomainErrorCode NSS0015 = DomainErrorCode("NSS0015", "Error setting public: '%s'");
const DomainErrorCode NSS0016 = DomainErrorCode("NSS0016", "Error setting format: '%s'");
const DomainErrorCode NSS0017 = DomainErrorCode("NSS0017", "Error setting protocol: '%s'");
const DomainErrorCode NSS0018 = DomainErrorCode("NSS0018", "Error setting non-blocking: '%s'");
const DomainErrorCode NSS0019 = DomainErrorCode("NSS0019", "Error connecting: '%s'");
const DomainErrorCode NSS0020 = DomainErrorCode("NSS0020", "Please specify playlist strategy (SIMPLE, RANDOM, RANDOM_ONCE)");
const DomainErrorCode NSS0021 = DomainErrorCode("NSS0021", "Maximum of error count reached. Give up..");
const DomainErrorCode NSS0022 = DomainErrorCode("NSS0022", "Libshout error occured. Playlist stopped");
const DomainErrorCode NSS0023 = DomainErrorCode("NSS0023", "NoiseStreamer is not running!");
const DomainErrorCode NSS0024 = DomainErrorCode("NSS0024", "Failed to reencode track '%s', no encode context found");
const DomainErrorCode NSS0025 = DomainErrorCode("NSS0025", "Incorrect requested track index");
const DomainErrorCode NSS0026 = DomainErrorCode("NSS0026", "NoiseStreamer is already running!");
const DomainErrorCode NSS0027 = DomainErrorCode("NSS0027", "Libshout queue length threshold reached '%s'");

#ifndef SETTING_H
#define SETTING_H

#include <cstdint>
#include <cstdio>

extern "C" {
#include "libconfig.h"
}

class Setting
{
public:
	Setting(const char* cfgFileName)
	{
		config_init(&cfg);
		uint8_t ret = config_read_file(&cfg, cfgFileName);
		if (!ret) {
			printf("Settings failed to load.\n");
		}
		else {
			printf("Settings loaded successfully.\n");
		}
	}

	~Setting()
	{
		config_destroy(&cfg);
	}

	bool getBool(const char* configPath)
	{
		int data = 0;
		int ret = config_lookup_bool(&cfg, configPath, &data);
		if (!ret)
			printf("Failed to access: '%s'\n", configPath);
		return (bool)data;
	}

	unsigned int getUInt(const char* configPath)
	{
		int data = 0;
		int ret = config_lookup_int(&cfg, configPath, &data);
		if (!ret)
			printf("Failed to access: '%s'\n", configPath);
		return (unsigned)data;
	}

	int getInt(const char* configPath)
	{
		int data = 0;
		int ret = config_lookup_int(&cfg, configPath, &data);
		if (!ret)
			printf("Failed to access: '%s'\n", configPath);
		return data;
	}

	long getInt32(const char* configPath)
	{
		long long data = 0;
		int ret = config_lookup_int64(&cfg, configPath, &data);
		if (!ret)
			printf("Failed to access: '%s'\n", configPath);
		return (long)data;
	}

	float getFloat(const char* configPath)
	{
		double data = 0.0f;
		int ret = config_lookup_float(&cfg, configPath, &data);
		if (!ret)
			printf("Failed to access: '%s'\n", configPath);
		return data;
	}

	config_t cfg;
	config_setting_t* setting;

public:
	void checkSetting()
	{
		// Do nothing.
	}
};

#endif
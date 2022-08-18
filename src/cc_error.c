#include "cc_internal.h"

struct CC_LogData
{
	uint64_t currentLevel;
	const char *outputPath;
	FILE *output;
	const char * const levelNames[5];
} g_log_data = 
{
	.currentLevel = CC_LOG_LEVEL_TRACE,
	.outputPath = NULL,
	.output = NULL,
	.levelNames = 
	{
		"TRACE",
		"INFO",
		"WARN",
		"ERROR",
		"UNKNOWN_LOG_LEVEL"
	}
};

void CC_Log_Log(uint64_t level, const char *file, uint64_t line, const char *format, ...)
{
	if(g_log_data.currentLevel <= level)
	{
		FILE *outputs[] = { g_log_data.output, stderr };

		va_list va_args;
		va_start(va_args, format);

		for(uint64_t i = 0; i < 2; i++)
		{
			if(outputs[i] != NULL)
			{
				fprintf(outputs[i], "[%s]\t%s:%llu\t", g_log_data.levelNames[(level > 4) ? 4 : level], file, line);

				vfprintf(outputs[i], format, va_args);

				fprintf(outputs[i], "\n");
			}
		}

		va_end(va_args);
	}
}

void CC_Log_SetFile(const char *path)
{
	if(g_log_data.output == NULL)
	{
		g_log_data.outputPath = path;
		g_log_data.output = fopen(path, "w");
	}
	else
	{
		CC_Log_Warn("CC_Log_SetFile has already been called.");
	}
}

uint64_t CC_Log_IsFileSet()
{
	return g_log_data.output != NULL;
}

void CC_Log_CloseFile()
{
	if(g_log_data.output != NULL)
	{
		fclose(g_log_data.output);
		g_log_data.output = NULL;
		g_log_data.outputPath = NULL;
	}
	else
	{
		CC_Log_Warn("Cannot run CC_Log_CloseFile because file hasn't been set.")
	}
}

void CC_Log_SetLevel(CC_Log_Level level)
{
	g_log_data.currentLevel = level;
}


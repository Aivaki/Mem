#pragma once

#include <filesystem>
#include <msclr\marshal_cppstd.h>
#include <random>
#include <vector>
#include <ctime>
#include "cheat/localPlayer.hpp"


/* All classes in the file */


/* Final product */
using namespace localPlayer;

/* Static global extension string */
static const std::string appExtension_stdString = ".mem";

#pragma region code
#ifndef codeDefine
#define codeDefine

namespace code
{

	/* Random name namespace */
	namespace randomName
	{

		/* Random name class */
		class RandomNameClass
		{
		private:
			/* Alphabet and numbers array */
			const typedef std::wstring alphabetAndNumbers;
			alphabetAndNumbers array = { L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890" };

			/* Function for generate */
			std::random_device randomDevice;

			/* Output variables */
			std::wstring randomString_wstring;
			std::string randomString_tmp;
			std::string randomString_exe;
			LPCSTR randomString_LPCSTR;
			std::string randomString_string;
			std::string randomString_name_string;

			/* Getting real time in seconds */
			int realTimeSeconds() noexcept(true) {
				time_t ttime = time(0);
				tm* local_time = localtime(&ttime);

				return local_time->tm_sec;
			}

			/* Getting today */
			int todayDate() noexcept(true) {
				time_t ttime = time(0);
				tm* local_time = localtime(&ttime);

				return local_time->tm_wday;
			}

			/* Generate the random name */
			void random() noexcept(true) {

				std::mt19937 Generator(randomDevice());
				std::uniform_int_distribution<> Distribution(0, this->array.size() - 1);

				for (std::size_t i = 0; i < this->array.size(); ++i)
					randomString_wstring += this->array[Distribution(Generator)];

				for (auto& Symbols : randomString_wstring)
					randomString_string += Symbols;
			}
		public:

			/* Functions with the random name */
			[[nodiscard]] const LPCSTR name_LPCSTR() noexcept(true) {
				random();
				return randomString_LPCSTR;
			}
			[[nodiscard]] const LPCSTR name_LPCSTR_exe() noexcept(true) {
				randomString_exe = randomString_string;

				random();

				/* Make a real random */
				randomString_exe.erase(todayDate() + 3, randomString_exe.size() - realTimeSeconds() + todayDate());

				randomString_exe += appExtension_stdString;
				randomString_LPCSTR = randomString_exe.c_str();
				return randomString_LPCSTR;
			}
			[[nodiscard]] const LPCSTR name_LPCSTR_tmp() noexcept(true) {
				random();

				randomString_tmp = randomString_string;

				/* Make a real random */
				randomString_tmp.erase(todayDate() + 3, randomString_tmp.size() - realTimeSeconds() + todayDate());

				randomString_tmp.insert(0, ".");

				randomString_LPCSTR = randomString_tmp.c_str();
				return randomString_LPCSTR;
			}
			[[nodiscard]] const LPCWSTR name_LPCWSTR() noexcept(true) {
				random();
				return randomString_wstring.c_str();
			}
			[[nodiscard]] const std::string name_string() noexcept(true) {
				random();

				randomString_name_string = randomString_string;

				/* Make a real random */
				randomString_name_string.erase(todayDate() + 3, randomString_name_string.size() - realTimeSeconds() + todayDate());

				return randomString_name_string;
			}
		};
	}

	/* App namespace */
	namespace myApp
	{
		/* App class */
		ref class AppClass : public System::Windows::Forms::Form
		{
		public:

			/* App mutex */
			[[nodiscard]] void appMutex() {
				if (!mutex)
					mutex = CreateMutex(NULL, true, appMutexName);

				if (mutex == nullptr)
					exit(0);

				/*  And now we are trying to capture */
				result = WaitForSingleObject(mutex, 0);

				/* If its acquisition fails, then the mutex is already acquired by someone else */
				if (result != WAIT_OBJECT_0)
					exit(0);
			}
			[[nodiscard]] void deleteAppMutex() {

				/* When the program is turned off, the mutex is released if it was captured */
				ReleaseMutex(mutex);

				/* And we remove it */
				CloseHandle(mutex);
			}

		private:
			/* App mutex */
			HANDLE mutex;
			DWORD result;

			const LPCSTR appMutexName = "a7WXYZ1c8STU6789Vde345fEFGHI2JKLMNsKLMR68b195XM4opq567lmnB1oKC346YZKLM213NLDEF1GpqHIJOPQ0";
		};
	}
}

#endif
#pragma endregion
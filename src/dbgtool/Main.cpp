#include <string>
#include <iostream>

#include <windows.h>
#include <dbghelp.h>
#include <stdio.h>

#pragma comment(lib, "dbghelp.lib")

char* get_name_from_path(char* path) {
	if(!path)
		return NULL;

	char* last_sl = strrchr(path, '\\');
	char* last_fw = strrchr(path, '/');

	char* name = path;
	if(last_sl && last_fw)
		name = (last_sl > last_fw) ? last_sl + 1 : last_fw + 1;
	else if(last_sl)
		name = last_sl + 1;
	else if(last_fw)
		name = last_fw + 1;

	return name;
}

int main(int argc, char** argv) {
	printf("--- SRB2: Delta Quest Debug Tool ---\nKeep in mind that, the .pdb file MUST have the same name as the .exe\n\n");
	
	std::string exe_path, pdb_path;

	printf("Enter .exe path: ");
	std::getline(std::cin, exe_path);

	/*std::string pdb_dir = ".";
	size_t last_sl = pdb_path.find_last_of("\\/");

	if(last_sl != std::string::npos)
		pdb_dir = pdb_path.substr(0, last_sl);*/

	HANDLE proc = GetCurrentProcess();
	SymSetOptions(SYMOPT_DEBUG | SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);

	// if(!SymInitialize(proc, pdb_dir.c_str(), TRUE)) {
	if(!SymInitialize(proc, NULL, TRUE)) {
		printf("SymInitialize failed: %lu\n", GetLastError());
		return 1;
	}

	// SymSetSearchPath(process, pdb_dir.c_str()); do we really need this?

	std::string exe_name = exe_path;
	size_t last_sl = exe_path.find_last_of("\\/");

	if(last_sl != std::string::npos)
		exe_name = exe_path.substr(last_sl + 1);

	DWORD64 base = SymLoadModuleEx(
		proc,
		NULL,
		exe_path.c_str(),
		exe_name.c_str(),
		0, 0,
		NULL,
		0
	);

	if(!base) {
		printf("SymLoadModuleEx failed: %lu\n", GetLastError());
		SymCleanup(proc);
		return 1;
	}

	IMAGEHLP_MODULE64 mod = {0};
	mod.SizeOfStruct = sizeof(mod);

	if(SymGetModuleInfo64(proc, base, &mod))
		printf("Loaded module: '%s'; You can now type addresses in hex (e.g., 0x00456E81)\nType 'exit' to quit\n\n", mod.LoadedPdbName);

	while(1) {
		std::string in;

		printf("> ");
		std::getline(std::cin, in);

		if(in == "exit")
			break;

		DWORD64 addr = _strtoui64(in.c_str(), NULL, 0);

		char buf[sizeof(SYMBOL_INFO) + 256] = {0};
		PSYMBOL_INFO sym = (PSYMBOL_INFO)buf;

		sym->SizeOfStruct = sizeof(SYMBOL_INFO);
		sym->MaxNameLen = 255;

		DWORD64 dis = 0;
		if(SymFromAddr(proc, addr, &dis, sym)) {
			IMAGEHLP_LINE64 l = {0};
			l.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

			DWORD l_dis = 0;
			if(SymGetLineFromAddr64(proc, addr, &l_dis, &l)) {
				printf(
					"0x%p -> %s+0x%I64X at %s:%lu (+0x%lX)\n",
					(void*)addr, sym->Name, dis,
					get_name_from_path(l.FileName), l.LineNumber, l_dis
				);
			} else
				printf("0x%p -> %s+0x%I64X\n", (void*)addr, sym->Name, dis);
		} else
			printf("0x%p -> <unknown> (Error: %lu)\n", (void*)addr, GetLastError());
	}

	SymUnloadModule64(proc, base);
	SymCleanup(proc);

	return 0;
}
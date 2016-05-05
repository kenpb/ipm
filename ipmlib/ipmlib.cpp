// This is the main DLL file.

#include "stdafx.h"

#include "ipmlib.h"

namespace ipmlib {

	void ipm::createISO(System::String^ path) {
		msclr::interop::marshal_context context;
		auto std_path = context.marshal_as<std::string>(path);
		slashReplace(std_path);
		auto std_isoName = getISOName(std_path);
		auto std_full = std_path + ISO_EXT;

		CString cstring_path = std_path.c_str(); // needed for allocsysstring
		BSTR bstr_path = cstring_path.AllocSysString(); // alloc bstr

		CString cstring_isoName = std_isoName.c_str(); // needed for allocsysstring
		BSTR bstr_isoName = cstring_isoName.AllocSysString(); // alloc bstr

		CoInitialize(NULL);

		STATSTG stg;
		ULONG size;
		FILE* f;

		IFileSystemImage* image = NULL;
		IFileSystemImageResult* result = NULL;
		IFsiDirectoryItem* root = NULL;
		IStream* file = NULL;
		IStream* r_i = NULL;

		CoCreateInstance(CLSID_MsftFileSystemImage, NULL, CLSCTX_ALL, __uuidof(IFileSystemImage), (void**)&image);
		image->put_FileSystemsToCreate((FsiFileSystems)(FsiFileSystemJoliet | FsiFileSystemISO9660));
		image->put_VolumeName(bstr_isoName);
		image->ChooseImageDefaultsForMediaType(IMAPI_MEDIA_TYPE_CDRW);
		image->get_Root(&root);
		root->AddTree(bstr_path, VARIANT_TRUE);
		image->CreateResultImage(&result);
		result->get_ImageStream(&r_i);
		r_i->Stat(&stg, 1);

		char* data = new char[(int)stg.cbSize.QuadPart];
		float chunksize = (float)stg.cbSize.QuadPart / 100;
		fopen_s(&f, std_full.c_str(), "wb");

		while (!r_i->Read(data, (ULONG)chunksize, &size)) // do real calculation ffs
		{
			if (size != 0) {
				fwrite(data, size, 1, f);
				progress = (progress == 100) ? progress : progress++;
			}
			else
				break;
		}
		fclose(f);

		delete data;
		r_i->Release();
		root->Release();
		result->Release();
		image->Release();

		CoUninitialize();

		std::cout << "Output file at: " << std_full << std::endl;
	}

}
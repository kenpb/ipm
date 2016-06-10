// This is the main DLL file.

#include "stdafx.h"

#include "ipmlib.h"

// Avoid STATSTG confusion losing itself in space and time, I should look into it... maybe later...
namespace Interop = System::Runtime::InteropServices;

namespace ipmlib {

	void ipm::CreateISO(System::String^ path, System::String^ outpath) {
		// get the iso name, use the folder name as the out file
		auto separators = gcnew array<System::Char>{'\\', '/'};
		auto aux1 = path->TrimEnd(separators);
		array<System::String^>^ path_array = aux1->Split(separators, System::StringSplitOptions::RemoveEmptyEntries);
		auto isoname_str = static_cast<System::String^>(path_array->GetValue(path_array->Length - 1)) + ".iso";
		System::IntPtr isoname_ip = Interop::Marshal::StringToBSTR(isoname_str);
		// create the BSTR from the pointer
		BSTR bstr_isoName = static_cast<BSTR>(isoname_ip.ToPointer()); 
		// fix pointer to avoid garbage collection
		pin_ptr<BSTR> b_isoName = &bstr_isoName;

		// get the full path in bstr
		// get the trimmed path string
		System::IntPtr path_ip = Interop::Marshal::StringToBSTR(aux1);
		// create the BSTR from the pointer
		BSTR bstr_path = static_cast<BSTR>(path_ip.ToPointer());
		// fix pointer to avoid garbage collection
		pin_ptr<BSTR> b_path = &bstr_path;

		char* outpath_full;
		// out put file path in case of outpath specified
		if (!System::String::IsNullOrEmpty(outpath)) {
			// delete separator if exists, well add it again, but better make sure
			auto aux2 = outpath->TrimEnd(separators);
			outpath_full = (char*)Interop::Marshal::StringToHGlobalAnsi(outpath + separators->GetValue(0) + isoname_str).ToPointer();
		} else {
			outpath_full = (char*)Interop::Marshal::StringToHGlobalAnsi(aux1 + separators->GetValue(0) + isoname_str).ToPointer();
		}

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
		image->ChooseImageDefaultsForMediaType(IMAPI_MEDIA_TYPE_MAX);
		image->get_Root(&root);
		root->AddTree(bstr_path, VARIANT_TRUE);
		image->CreateResultImage(&result);
		result->get_ImageStream(&r_i);
		r_i->Stat(&stg, 1);

		char* data = new char[(int)stg.cbSize.QuadPart];
		float chunksize = (float)stg.cbSize.QuadPart / 100;
		fopen_s(&f, outpath_full, "wb");

		while (!r_i->Read(data, (ULONGLONG)chunksize, &size)) { // do real calculation ffs
			if (size != 0) {
				fwrite(data, size, 1, f);
				if (progress != 100) {
					progress++;
				}
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

		System::Console::WriteLine("Output file at: {0}", aux1 + "/" + isoname_str);

		Interop::Marshal::FreeBSTR(isoname_ip);
		Interop::Marshal::FreeBSTR(path_ip);
	}

}

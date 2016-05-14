// ipmlib.h

#pragma once

namespace ipmlib {

	public ref class ipm
	{
	public:
		ipm() { progress = 0; };
		~ipm() {};
		void CreateISO(System::String^ path, System::String^ outpath);
		System::UInt16 GetProgress() { return progress; }
	private:
		System::UInt16 progress;
	};
}

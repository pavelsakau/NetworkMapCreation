#include "BitmapHelper.h"

wxString BitmapHelper::GetResourceById(const unsigned int & res_id)
{
	return wxString::Format(wxT("#%i"), res_id);
}

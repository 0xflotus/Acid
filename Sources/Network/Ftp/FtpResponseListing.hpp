#pragma once

#include <vector>
#include "FtpResponse.hpp"

namespace acid
{
	///
	/// \brief Specialization of FTP response returning a filename listing.
	///
	class FtpResponseListing :
		public FtpResponse
	{
	private:
		std::vector<std::string> m_listing; /// Directory/file names extracted from the data.
	public:
		///
		/// \brief Default constructor.
		///
		/// \param response  Source response.
		/// \param data      Data containing the raw listing.
		///
		FtpResponseListing(const FtpResponse &response, const std::string &data);

		///
		/// \brief Return the array of directory/file names.
		///
		/// \return Array containing the requested listing.
		///
		const std::vector<std::string> &GetListing() const;
	};
}

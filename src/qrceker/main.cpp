#include <filesystem>
#include <fstream>
#include <ranges>
#include <sstream>

namespace
{

void Process(std::ostream& stream, const std::filesystem::path& path, const std::string& prefix)
{
	const auto delimiter = prefix.empty() ? "" : "/";

	std::vector<std::string> files;
	std::ranges::transform(std::filesystem::directory_iterator(path) | std::views::filter([](const auto& item) { return !item.is_directory(); }),
	                       std::back_inserter(files),
	                       [](const auto& item) { return item.path().filename().string(); });

	if (!files.empty())
	{
		stream << std::format("\t<qresource prefix=\"{}\">\n", prefix);

		for (const auto& file : files)
			stream << std::format("\t\t<file alias=\"{}\">{}{}{}</file>\n", file, prefix, delimiter, file);

		stream << "\t</qresource>\n";
	}

	for (const auto& entry : std::filesystem::directory_iterator(path))
		if (entry.is_directory())
			Process(stream, entry.path(), std::format("{}{}{}", prefix, delimiter, entry.path().filename().string()));
}

} // namespace

int main(const int argc, char* argv[])
{
	const auto path = argc > 1 ? std::filesystem::path(argv[1]) : std::filesystem::current_path();
	std::ostringstream stream;
	stream << "<RCC>\n";
	Process(stream, path, {});
	stream << "</RCC>\n";

	const auto name = path.filename().string();
	std::ofstream((path / name).replace_extension(".qrc")) << stream.str();

	return 0;
}

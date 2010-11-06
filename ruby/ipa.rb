# generates another ruby script to build an IPA for app store.
# requires some environment variables -- see script.

if !((ENV['BUILD_STYLE'].include?("Distribution")) || ENV['BUILD_STYLE'].include?("AppStore"))
	puts "not a distribution build -- nothing more to do in build script"
	Process.exit
end

major_version = "#{ENV['MAJOR_VERSION']}"
minor_version = "#{ENV['MINOR_VERSION']}"
distribution_version = "#{ENV['DISTRIBUTION_VERSION']}"

version_string = [major_version.strip(), minor_version.strip(), distribution_version.strip()].join(".")

# generate the IPA making script
ipa_script = %{#!/usr/bin/ruby
%x[rm -drf #{ENV['BUILT_PRODUCTS_DIR']}/Payload]
%x[mkdir #{ENV['BUILT_PRODUCTS_DIR']}/Payload]
%x[cp -R #{ENV['CODESIGNING_FOLDER_PATH']} #{ENV['BUILT_PRODUCTS_DIR']}/Payload]
%x[cd #{ENV['BUILT_PRODUCTS_DIR']} && zip #{ENV['PRODUCT_NAME']}_#{ENV['BUILD_STYLE']}_#{version_string}.ipa Payload/ -q -r]
%x[cp #{ENV['BUILT_PRODUCTS_DIR']}/#{ENV['PRODUCT_NAME']}_#{ENV['BUILD_STYLE']}_#{version_string}.ipa #{ENV['PROJECT_DIR']}]
}

script_file_handle = "#{ENV['PROJECT_DIR']}/makeIPA_#{version_string}.rb"
File.open(script_file_handle, 'w') {|f| f.write(ipa_script) }

puts "Build script successful. Updated version to " + version_string 
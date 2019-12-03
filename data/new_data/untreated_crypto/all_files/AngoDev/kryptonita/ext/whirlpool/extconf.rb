ENV['RC_ARCHS'] = '' if RUBY_PLATFORM =~ /darwin/

require "mkmf"

RbConfig::MAKEFILE_CONFIG['CC'] = ENV['CC'] if ENV['CC']

dir_config("whirlpool")

if RbConfig::CONFIG["host_os"] =~ /mswin/
  $CFLAGS << ' -W3'
end

#create_makefile "whirlpool"
create_makefile "whirlpool/whirlpool"

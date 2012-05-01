$: << File.dirname(__FILE__)

require "tool/compile.rb"
require "tool/test.rb"

task :ctags do
  sh "ctags -R ./src"
end

task :default => [:ctags, :run]



task :test => [:compile] do
  sh "ruby test/test.rb"
end

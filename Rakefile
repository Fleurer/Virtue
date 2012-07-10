def omap(path)
  "bin/#{File.basename(path).ext('o')}"
end

CC = 'gcc'
CFLAG = '-Wall -Werror'
CINC = ''

src_cfiles = FileList['src/*.c']
src_hfiles = FileList['src/*.h']
src_ofiles = src_cfiles.map{|path| omap(path) }
test_cfiles = FileList['test/test_*.c']
test_ofiles = test_cfiles.map{|path| omap(path) }
test_exefiles = test_cfiles.map{|path| omap(path).ext('') }

(src_cfiles + test_cfiles).each do |cfile|
  ofile = omap(cfile)
  file ofile => [cfile, *src_hfiles] do 
    sh "#{CC} #{CFLAG} #{CINC} -g -c #{cfile} -o #{ofile} 2>&1"
  end
end

file 'bin/vt' => src_ofiles do 
  sh "#{CC} -g -o bin/vt #{src_ofiles}"
end

test_ofiles.each do |ofile|
  exe_file = ofile.ext('')
  ofiles = [ofile] + src_ofiles - ['bin/main.o']
  file exe_file => ofiles do 
    sh "#{CC} -g -o #{exe_file} #{ofiles*' '}"
  end
end

task :compile => ['bin/vt', *test_exefiles]

task :test => test_exefiles do 
  test_exefiles.each do |exefile|
    sh exefile
  end
end

task :run => :compile do
  sh "bin/vt"
end

task :clean do
  sh "rm bin/* -rf"
end

task :ctags do
  sh "ctags -R ./src"
end

task :default => [:ctags, :test]


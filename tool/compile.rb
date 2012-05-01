def omap(path)
  "bin/#{File.basename(path).ext('o')}"
end

CC = 'gcc'
CFLAG = '-Wall -Werror'
CINC = ''

cfiles = FileList['src/*.c']
hfiles = FileList['src/vt/*.h']
ofiles = cfiles.map{|path| omap(path) }

cfiles.each do |cfile|
  ofile = omap(cfile)
  file ofile => [cfile, *hfiles] do 
    sh "#{CC} #{CFLAG} #{CINC} -c #{cfile} -o #{ofile} 2>&1"
  end
end

file 'bin/vt' => ofiles do 
  sh "#{CC} -o bin/vt #{ofiles}"
end

task :compile => 'bin/vt'

task :run => :compile do
  sh "bin/vt"
end

task :clean do
  sh "rm bin/* -rf"
end

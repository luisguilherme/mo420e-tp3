#!/usr/bin/env ruby
  
Dir['input/*.sta'].each do |file|
  ['b','r','f'].each do |method| 
    0.upto(1) do |primal|
      c = [0]
      c << 1 if method != 'b'
      c.each do |cut|
        instname = File.basename(file,'.sta')
        id = "#{method}#{primal}#{cut}"
        timefile = "output/#{instname}-#{id}.tm"
        plotfile = "output/#{instname}-#{id}.dat"
        logfile = "output/#{instname}-#{id}.log"
        timing = "/usr/bin/time -p -o #{timefile}" 
        cmd = "./bnc #{method} 3600 #{primal} #{cut} #{file}"
        puts "Running #{instname} #{id}"
        system "#{timing} #{cmd} > #{logfile} 2> #{plotfile}"
      end
    end
  end
end    
  

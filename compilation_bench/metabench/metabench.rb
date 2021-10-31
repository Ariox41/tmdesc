# Copyright Victor Smirnov 2021
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
#
# This file is part of the simple compile-time benchmark tools for the tmdesc library. 
# The original idea is taken from https://github.com/ldionne/metabench
require 'benchmark'
require 'erb'
require 'fileutils'
require 'open3'
require 'pathname'
require 'time'

def build(target, cpp_file, compilation_command, exe_file)
    # command = [cmake_command, '--build', '.', '--target', target]
    command = compilation_command

    FileUtils.touch(cpp_file, mtime: Time.now)  
    File.delete(exe_file) if File.exist?(exe_file) 

    stdout, stderr, status = nil

    
    realtime = Benchmark.realtime  {                                                       
        stdout, stderr, status = Open3.capture3(compilation_command)                         
    } 
    # for i in 0..10
    #     stdout, stderr, status = Open3.capture3(*command) 
    #     break if status.success?
    #     ninja_error = stderr.include?("ninja: error: opening build log: Permission denied")
    #     $stderr.write("\nninja error detected: #{i}") if ninja_error
    #     break unless ninja_error
    #     sleep(0.1)
    # end
    
    raise "\nError while c++ file compilation\n\nstdout:\n#{stdout}\n\nstderr:\n#{stderr}\n" unless status.success?

    #match = stdout.match(/\[metabench compilation time: (.+)\]/i)
    # raise "\nCould not find [metabench compilation time: ...] in the output. Are you using a Ninja or Make generator?\n\nFull stdout:\n" + stdout  if match.nil?
    #realtime = match.captures[0].to_f

    return realtime
end

class TemplateArgs
    def initialize( item )
      @item = item
    end
  
    def get_binding
      binding
    end
  end

def apply_template(erb_template, item)                                                                         
    begin   
        args = TemplateArgs.new(item)                                                                                               
        ERB.new(File.read(erb_template)).result(args.get_binding)                                                     
    rescue => e               
        raise "C++ file generation error. ERB template: #{erb_template}\nCurrent item: #{item.to_s}\nError:\n#{e.full_message}"                                                                                              
    end  
end 

def measure(target, range_expr_str, erb_template, cpp_file,  repetitions, exe_file, compilation_command_file)
    begin
        range = eval(range_expr_str).to_a
        exe_file = exe_file.gsub('.tmdesc_metabench_pattern.cpp', "")
    
        out = {}
        out['target'] = target
        out['range'] = range
        out['data'] = []

        compilation_command = IO.read(compilation_command_file)
    
        $stderr.write("### tmdesc metabench #{target}..... 0/#{range.size}")
        range.each_with_index do |n, index|
            measure_code_compilation_time = -> (code){
                IO.write(cpp_file, code)
            
                repetitions.times.map {
                    build(target, cpp_file, compilation_command, exe_file)
                }
            }
           
            code = apply_template(erb_template, n)
            measure_code_compilation_time[code] if index == 0 # generate cache on first iteration

            base_times = measure_code_compilation_time[code]


            total_times = measure_code_compilation_time["#define METABENCH\n#{code}"]

            out['data'] << {
                'input' => n,
                'index' => index,
                'base_times' => base_times,
                'total_times' => total_times
            }
            $stderr.write("\r### tmdesc metabench #{target}..... #{index + 1}/#{range.size}")
        end
        $stderr.write("\n")

        return out
    rescue => e
        $stderr.puts("\n\nMeasure error for target '#{target}':\n")
        raise e    
    end
end

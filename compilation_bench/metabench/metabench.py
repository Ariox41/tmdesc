from typing import Sequence, Tuple, Dict
from collections.abc import Iterable
from pathlib import Path
from shutil import rmtree
from mako.template import Template
from statistics import mean
import subprocess
import random
import time
import math


class MeasureResult:
    def __init__(self, item):
        self.item = item
        self.results: Sequence[float] = []

    def append(self, time: float):
        self.results.append(time)

    def finalize(self):
        self.mean_time = mean(self.results)
        self.min = min(self.results)
        self.max = max(self.results)

    def sum_time(self):
        return math.fsum([.0] + self.results)


class CMakeMetaBench:
    template: str
    find_packages: Sequence[str]
    link_libraries: Sequence[str]
    include_directories: Sequence[str]
    build_dir: Path

    def __init__(self,
                 template: str,
                 find_packages: Sequence[str] = None,
                 include_directories: Sequence[str] = None,
                 link_libraries: Sequence[str] = None,
                 build_dir: Path = Path("build/metabench"),
                 cmake_command: str = "cmake",
                 cmake_args: Sequence[str] = [],
                 cmake_build_type: str = "Release"
                 ):
        """ Initializes the benchmark for the .cpp file template

        Args:
            template (str): .cpp file template with the [Mako Templates syntax](https://www.makotemplates.org/)
            find_packages (Sequence[str], optional): The list of packages for CMake find_package. Defaults to None.
            include_directories (Sequence[str], optional): The list of path for CMake target_include_directories. Defaults to None.
            link_libraries (Sequence[str], optional):  The list of libraries for CMake target_link_libraries. Defaults to None.
        """
        self.template = template
        self.find_packages = find_packages if find_packages else []
        self.include_directories = include_directories if include_directories else []
        self.link_libraries = link_libraries if link_libraries else []
        self.build_dir = build_dir
        self.cmake_command = cmake_command
        self.cmake_args = cmake_args
        self.cmake_build_type = cmake_build_type

    def measure(self, range: Iterable, number: int = 3, max_time: float = None) -> Tuple[Sequence[str], Sequence[str]]:
        self.configure()
        sorted_items = list(map(MeasureResult, range))
        items = list(sorted_items)
        for _ in range(0, number):
            random.shuffle(items)
            for item in items:
                if max_time and item.sum_time() > max_time:
                    continue
                item.append(self.measure_one(item.item))

        for item in items:
            item.finalize()
        return sorted_items

    def configure(self):
        if self.build_dir.exists():
            rmtree(self.build_dir)
        self.build_dir.mkdir(parents=True)
        tmp_dir = self.build_dir / "build"
        tmp_dir.mkdir()

        cmakelists_path = self.build_dir / Path("CMakeLists.txt")
        cpp_file_path = self.build_dir / Path("bench_src.cpp")
        cpp_file_path.touch()
        with cmakelists_path.open('w') as f:
            f.write(self.render_cmakefile(self.find_packages,
                    self.include_directories, self.link_libraries))

        subprocess.run([self.cmake_command, "..",
                       f"-DCMAKE_BUILD_TYPE={self.cmake_build_type}"] + self.cmake_args, check=True, cwd=tmp_dir)
        return self

    def measure_one(self, item):
        tmp_dir = self.build_dir / "build"
        cpp_file_path = self.build_dir / Path("bench_src.cpp")
        if not tmp_dir.exists():
            raise Exception("CMakeMetaBench.configure() required")
        for file in tmp_dir.glob("metabench*"):
            if file.is_file():
                file.unlink()

        cppdata = Template(text=self.template).render(item=item)
        with cpp_file_path.open('w') as f:
            f.write(cppdata)
        start_time = time.monotonic()
        subprocess.run([self.cmake_command, "--build", "--target",
                       "metabench", "--parallel", "1"], check=True, cwd=tmp_dir)
        return time.monotonic() - start_time

    def render_cmakefile(find_packages: Sequence[str], include_directories: Sequence[str], link_libraries: Sequence[str]):
        cmakelists_template = ('''
cmake_minimum_required (VERSION 3.7)
project (metabench LANGUAGES CXX)
                               
% for fp in find_packages:
${f"find_package({fp})"}
% endfor

add_executable(metabench "bench_src.cpp")

target_link_libraries(metabench PRIVATE 
% for dir in include_directories:
${f'"{dir}"'}
% endfor
)

target_link_libraries(metabench PRIVATE
% for lib in link_libraries:
${f'"{lib}"'}
% endfor
)
''')
        return Template(cmakelists_template).render(find_packages=find_packages, include_directories=include_directories, link_libraries=link_libraries)

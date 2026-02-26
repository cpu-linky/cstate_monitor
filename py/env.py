import os
from dotenv import load_dotenv
from pathlib import Path

dotenv_path = Path(__file__).resolve().parent.parent / "config.env"
load_dotenv(dotenv_path)

CPU_TARGET_LOAD = os.getenv("CPU_TARGET_LOAD")
CPU_TARGET_TURBOSTAT = os.getenv("CPU_TARGET_TURBOSTAT")

CPU_N_PI = os.getenv("CPU_N_PI")
N_CPU = os.getenv("N_CPU")
CPU_LOG_PATH = os.getenv("CPU_LOG_PATH")

MEMORY_N_JUMPS = os.getenv("MEMORY_N_JUMPS")
N_MEMORY = os.getenv("N_MEMORY")
MEMORY_LOG_PATH = os.getenv("MEMORY_LOG_PATH")

IO_N_DUMPS = os.getenv("IO_N_DUMPS")
N_IO = os.getenv("N_IO")
IO_LOG_PATH = os.getenv("IO_LOG_PATH")

SLEEP_LOG_PATH = os.getenv("SLEEP_LOG_PATH")
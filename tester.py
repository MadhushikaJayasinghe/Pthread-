import os
import sys
import json

import statistics as stats

from decimal import Decimal
from subprocess import check_call, check_output, CalledProcessError

# compile c source files
try:
    check_call(["gcc", "-g", "-Wall", "-o", "serial", "serial.c", "-lpthread", "-lm"])
    print("Successfully compiled serial.c")
except CalledProcessError:
    print("Error in compiling serial.c")

try:
    check_call(["gcc", "-g", "-Wall", "-o", "parallel_lock", "parallel_lock.c", "-lpthread", "-lm"])
    print("Successfully parallel_lock.c")
except CalledProcessError:
    print("Error in compiling parallel_lock.c")

try:
    check_call(["gcc", "-g", "-Wall", "-o", "parallel_mutex", "parallel_mutex.c", "-lpthread", "-lm"])
    print("Successfully compiled parallel_mutex.c")
except CalledProcessError:
    print("Error in compiling parallel_mutex.c")


def get_stats(cmd, cycles=35):
    serial_ts = []
    for _ in range(cycles):
        print(cmd)
        sys.stdout.flush()
        elapsed_t = check_output(cmd, shell=False).decode('UTF-8')
        serial_ts.append(Decimal(elapsed_t))
    mean = stats.mean(serial_ts)
    std = stats.pstdev(serial_ts)
    return {
        "mean": '{0:.9f}'.format(mean),
        "sd": '{0:.9f}'.format(std)
    }


def get_stats_for_case(case, case_id, prog):
    case_stats = dict()
    for thread in ["1", "4", "8"]:
        test_case = case[:]
        if prog == "serial":
            if int(thread) > 1:
                break
        else:
            test_case += [thread]

        stat_result = get_stats(["./{0}".format(prog)] + test_case)
        case_stats[thread] = stat_result
    os.makedirs("results", exist_ok=True)
    with open("results/case_{0}-{1}-stats.json".format(case_id, prog), "w") as fn:
        json.dump(case_stats, fn)
        print("dumped results/case_{0}-{1}-stats.json".format(case_id, prog))
    return case_stats


# n = 1,000 and m = 10,000, mMember = 0.99, mInsert = 0.005, mDelete = 0.005
case1 = ["1000", "10000", "0.99", "0.005", "0.005"]
# print(get_stats_for_case(case1, 1, "serial"))
# print(get_stats_for_case(case1, 1, "parallel_mutex"))
# print(get_stats_for_case(case1, 1, "parallel_lock"))

# n = 1,000 and m = 10,000, mMember = 0.90, mInsert = 0.05, mDelete = 0.05
case2 = ["1000", "10000", "0.90", "0.05", "0.05"]
# print(get_stats_for_case(case2, 2, "serial"))
# print(get_stats_for_case(case2, 2, "parallel_mutex"))
# print(get_stats_for_case(case2, 2, "parallel_lock")) # prob

# n = 1,000 and m = 10,000, mMember = 0.50, mInsert = 0.25, mDelete = 0.25
case3 = ["1000", "10000", "0.50", "0.25", "0.25"]
# print(get_stats_for_case(case3, 3, "serial"))
# print(get_stats_for_case(case3, 3, "parallel_mutex"))
# print(get_stats_for_case(case3, 3, "parallel_lock"))

overall = dict()
for c in ["case_1", "case_2", "case_3"]:
    overall[c] = dict()
    for prog in ["serial", "parallel_mutex", "parallel_lock"]:
        with open("results/{0}-{1}-stats.json".format(c, prog)) as fn:
            overall[c][prog] = json.load(fn)
with open("results/all-stats.json", "w") as fn:
    print("dumped results/all-stats.json")
    json.dump(overall, fn)

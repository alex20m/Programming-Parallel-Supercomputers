import argparse
import os
def preprocess_file(input_file, exercise_file, solution_file):
    exercise_file = exercise_file.replace("_EXERCISE","")
    solution_file = solution_file.replace("_SOLUTION","")
    if any([x in input_file for x in [".git"]]):
        if ".gitignore" not in input_file:
            return

    print("INPUT:", input_file)

    if any([x in input_file for x in [".pdf",".png"]]):
        if "_EXERCISE" in input_file:
            os.popen(f"cp {input_file} {exercise_file}")
        elif "_SOLUTION" in input_file:
            os.popen(f"cp {input_file} {solution_file}")
        else:
            os.popen(f"cp {input_file} {exercise_file}")
            os.popen(f"cp {input_file} {solution_file}")
        return

    with open(input_file, 'r') as infile:
     lines = infile.readlines()


    exercise_lines = []
    solution_lines = []

    in_exercise = True 
    in_solution = True 

    for line in lines:


        if line.strip() == '#START EXERCISE':
            in_exercise = True
            in_solution = False
        elif line.strip() == '#END EXERCISE':
            in_exercise = True 
            in_solution = True 
        elif line.strip() == '#START SOLUTION':
            in_exercise = False
            in_solution = True
        elif line.strip() == '#END SOLUTION':
            in_exercise = True 
            in_solution = True 
        else:

            if "_EXERCISE" in input_file:
                in_exercise = True
                in_solution = False
            elif "_SOLUTION" in input_file:
                in_exercise = False 
                in_solution = True 

            if in_exercise:
                exercise_lines.append(line)
            if in_solution:
                solution_lines.append(line)


    if(len(exercise_lines) > 0):
        with open(exercise_file, 'w') as exfile:
            exfile.writelines(exercise_lines)

    if(len(solution_lines) > 0):
        with open(solution_file, 'w') as solfile:
            solfile.writelines(solution_lines)

def main():
    argparser = argparse.ArgumentParser(description="Helper script to generate exercise and solution files from base file",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    argparser.add_argument("-D", "--directory", help="root directory to parse files", required=True)
    argparser.add_argument("-E", "--ex_dir",  help="directory to output exercises", required=True)
    argparser.add_argument("-S", "--sol_dir", help="directory to output solutions", required=True)
    root_dir = vars(argparser.parse_args())["directory"]
    ex_dir   = vars(argparser.parse_args())["ex_dir"]
    sol_dir  = vars(argparser.parse_args())["sol_dir"]
    cur_dir = os.getcwd()
    print(cur_dir)
    if(root_dir not in cur_dir):
        print("ERROR: you need to run this in the source directory")
        exit()
    #copy the folder structure to the source directories
    os.system(f"find . -type d | grep -v '^\.$' | cpio -pdm {ex_dir}")
    os.system(f"find . -type d | grep -v '^\.$' | cpio -pdm {sol_dir}")
    files = [f for f in os.listdir(root_dir) if os.path.isfile(f)]
    for subdir, dirs, files in os.walk(root_dir):
        for file in files:
            full_path = f"{subdir}/{file}"
            exercise_file = full_path.replace(root_dir,ex_dir)
            solution_file = full_path.replace(root_dir,sol_dir)
            preprocess_file(full_path, exercise_file, solution_file)


if __name__ == "__main__":
    main()



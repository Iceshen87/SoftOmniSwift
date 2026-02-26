use crate::arguments::arguments;
use crate::arguments::arguments::{FileFolderItemLocation, SortingOrder};
use std::cmp::Ordering;
use std::fs;
use std::fs::{DirEntry, ReadDir};
use std::path::Path;

pub fn process(arguments: &arguments::Arguments) -> usize {
    if arguments.path().is_none() {
        eprintln!(
            "Path argument is required for processing.\n\
        Argument validation and fetching and setting up failed: Path is missing."
        );
        return 0;
    }

    if arguments.is_file_target() {
        return process_file(arguments);
    }

    let path_str: &str = arguments.path().as_deref().unwrap();
    process_root_directory(arguments, Path::new(path_str))
}

fn process_root_directory(arguments: &arguments::Arguments, path: &Path) -> usize {
    let result = path.read_dir();
    if result.is_err() {
        eprintln!(
            "Skipping directory {} as an error occurred when attempting to retrieve its children\n\
            The error was: {}",
            path.to_str().unwrap(),
            result.err().unwrap()
        );

        return 0;
    }

    let mut read_dir = result.unwrap();

    if !matches!(
        arguments.file_folder_item_location(),
        FileFolderItemLocation::InOrderOfSortingAlgorithm
    ) {
        process_directory_in_order_of_sorting_algorithm(
            arguments,
            &mut read_dir,
            arguments.max_depth(),
            path,
        )
    } else {
        process_directory_as_chunks(
            arguments,
            &mut read_dir,
            arguments.max_depth(),
            path,
        )
    }
}

fn process_file(arguments: &arguments::Arguments) -> usize {
    let file = arguments.path().as_deref().unwrap();
    let path = Path::new(file);

    let path_buffer = path.to_path_buf();
    let potential_parent_buffer = fs::canonicalize(path_buffer.clone());

    if potential_parent_buffer.is_err() {
        let potential_name = path_buffer.to_str();

        if potential_name.is_none() {
            eprintln!("Error: the parent directory of the file was removed after argument parsing");
            return 0;
        }

        eprintln!("Error: the parent directory of the file {} was removed after argument parsing",
                  potential_name.unwrap());
        return 0;
    }

    let parent_buffer = potential_parent_buffer.unwrap();
    let potential_directory_reader = parent_buffer.read_dir();
    if potential_directory_reader.is_err() {
        eprintln!("Error: failed to read directory contents of parent directory {} because of {:?}",
                  parent_buffer.display(), potential_directory_reader.unwrap_err());
        return 0;
    }

    let mut directory_reader = potential_directory_reader.unwrap();

    if !matches!(arguments.file_folder_item_location(), FileFolderItemLocation::InOrderOfSortingAlgorithm) {
        process_directory_in_order_of_sorting_algorithm(arguments, &mut directory_reader, 1, path)
    } else {
        process_directory_as_chunks(arguments, &mut directory_reader, 1, path)
    }
}

enum LoopSubCallReturnBehavior {
    NormalProcessing,
    Break,
    Continue,
}

fn process_directory_in_order_of_sorting_algorithm(
    arguments: &arguments::Arguments,
    read_dir: &mut ReadDir,
    recursion_depth_limit: usize,
    path: &Path,
) -> usize {
    if recursion_depth_limit == 0 {
        if arguments.is_file_target() {
            return 0;
        }

        eprintln!(
            "Warning: Recursion depth limit reached. Skipping directory processing of {:?}",
            path
        );
        return 0;
    }

    let mut entries: Vec<DirEntry> = Vec::new();

    loop {
        let current_entry = read_dir.next();
        if current_entry.is_none() {
            break;
        }

        match current_entry.unwrap() {
            Ok(entry) => entries.push(entry),
            Err(error) => eprintln!("Error reading directory entry: {:?}. Skipping", error),
        }
    }

    let mut buffer= String::new();
    let number_of_files_inserted_in_children: usize = 0;
    let mut number_of_files_inserted_in_current_directory: usize = 0;

    sort_entries(&mut entries, &arguments.sorting_order());

    let mut entry_index: usize = 0;
    while entry_index < entries.len() {
        let entry = &entries[entry_index];
        let potential_file_type = entry.file_type();

        if potential_file_type.is_err() {
            eprintln!(
                "Error determining file type for entry: {:?}. Skipping",
                entry.file_name()
            );
            continue;
        }

        let file_type = potential_file_type.unwrap();

        if file_type.is_dir() {
            process_directory(
                arguments,
                &mut buffer,
                entry,
                process_directory_in_order_of_sorting_algorithm,
                recursion_depth_limit,
                path.join(entry.file_name()).as_path(),
            );

            entry_index += 1;
        } else {
            let mut file_group: Vec<String> = Vec::new();
            let mut file_group_length: usize =
                arguments.source_variable_name().len() + " += files(".len();

            let result = gather_consecutive_file_group(
                arguments,
                &mut entry_index,
                &entries,
                &mut file_group,
                &mut file_group_length,
                path.join(entry.file_name()).as_path(),
            );

            if matches!(result, LoopSubCallReturnBehavior::Break) {
                break;
            } else if matches!(result, LoopSubCallReturnBehavior::Continue) {
                continue;
            }

            write_file_group(arguments, &mut buffer, &file_group, file_group_length);

            number_of_files_inserted_in_current_directory += file_group_length;
        }
    }

    number_of_files_inserted_in_children + number_of_files_inserted_in_current_directory
}

fn process_directory_as_chunks(
    arguments: &arguments::Arguments,
    read_dir: &mut ReadDir,
    recursion_depth_limit: usize,
    path: &Path,
) -> usize {
    if recursion_depth_limit == 0 {
        if arguments.is_file_target() {
            return 0;
        }

        eprintln!(
            "Warning: Recursion depth limit reached. Skipping directory processing of {:?}",
            path
        );
        return 0;
    }

    let mut buffer = String::new();
    let mut file_entries: Vec<DirEntry> = Vec::new();
    let mut directory_entries: Vec<DirEntry> = Vec::new();

    loop {
        let current_entry = read_dir.next();
        if current_entry.is_none() {
            break;
        }

        match current_entry.unwrap() {
            Ok(entry) => {
                let potential_file_type = entry.file_type();

                if potential_file_type.is_err() {
                    eprintln!(
                        "Error determining file type for entry: {:?}. Skipping",
                        entry.file_name()
                    );
                    continue;
                }

                let file_type = potential_file_type.unwrap();

                if file_type.is_file() {} else if file_type.is_dir() {
                    directory_entries.push(entry);
                }
            }
            Err(error) => eprintln!("Error reading directory entry: {:?}. Skipping", error),
        }
    }

    sort_entries(&mut file_entries, &arguments.sorting_order());
    sort_entries(&mut directory_entries, &arguments.sorting_order());

    let mut number_of_files_changed: usize = 0;

    for directory_entry in directory_entries {
        number_of_files_changed += process_directory(
            arguments,
            &mut buffer,
            &directory_entry,
            process_directory_as_chunks,
            recursion_depth_limit,
            path.join(directory_entry.file_name()).as_path(),
        );
    }

    let mut file_group_length: usize = arguments.source_variable_name().len() + " += files(".len();
    let file_entries_shortened: Vec<String> = file_entries
        .iter()
        .filter_map(|entry| {
            let os_file_name = entry.file_name();
            let potential_file_name = os_file_name.to_str();
            if potential_file_name.is_none() {
                eprintln!(
                    "Error: Could not convert file name to string for entry: {:?}. Skipping...",
                    entry
                );
                return None;
            }

            file_group_length += potential_file_name.unwrap().len();
            Some(potential_file_name.unwrap().to_string())
        })
        .collect();

    write_file_group(
        arguments,
        &mut buffer,
        &file_entries_shortened,
        file_group_length,
    );

    number_of_files_changed += file_entries_shortened.len();
    number_of_files_changed
}

fn process_directory<SubFunctionRecursiveCall>(
    arguments: &arguments::Arguments,
    buffer: &mut String,
    directory_entry: &DirEntry,
    recursive_call: SubFunctionRecursiveCall,
    recursion_depth_limit: usize,
    path: &Path,
) -> usize
where
    SubFunctionRecursiveCall:
    Fn(&arguments::Arguments, &mut ReadDir, usize, &Path) -> usize,
{
    let potential_child_read_dir = directory_entry.path().read_dir();
    if potential_child_read_dir.is_err() {
        eprintln!(
            "Error opening child directory: {:?}. Skipping",
            directory_entry.file_name()
        );
        return 0;
    }

    let os_file_name = directory_entry.file_name();
    let potential_file_name_str = os_file_name.to_str();
    if potential_file_name_str.is_none() {
        // This should never happen
        eprintln!(
            "Error converting file name to string for entry: {:?}. Skipping",
            directory_entry.file_name()
        );
        return 0;
    }

    let mut child_read_dir = potential_child_read_dir.unwrap();
    let number_of_files_changed: usize = recursive_call(
        arguments,
        &mut child_read_dir,
        recursion_depth_limit - 1,
        path,
    );

    buffer.push_str(&format!(
        "\nsubdir('{}')\n",
        potential_file_name_str.unwrap()
    ));

    number_of_files_changed
}

fn write_file_group(
    arguments: &arguments::Arguments,
    buffer: &mut String,
    file_group: &Vec<String>,
    file_group_length: usize,
) {
    if arguments.forcefully_put_on_distinct_lines()
        && file_group_length <= arguments.max_line_length()
    {
        buffer.push_str("files(");
        for (i, file_name) in file_group.iter().enumerate() {
            buffer.push_str(file_name);
            if i < file_group.len() - 1 {
                buffer.push_str(", ");
            }
        }

        buffer.push_str(")\n");
        return;
    }

    buffer.push_str("files(\n");

    for (index, file_name) in file_group.iter().enumerate() {
        buffer.push_str("    ");
        buffer.push_str(file_name);
        if index < file_group.len() - 1 {
            buffer.push_str(",\n");
        }
    }

    buffer.push_str("\n)\n");
}

fn gather_consecutive_file_group(
    arguments: &arguments::Arguments,
    index: &mut usize,
    entries: &Vec<DirEntry>,
    file_group: &mut Vec<String>,
    file_group_length: &mut usize,
    directory_path: &Path,
) -> LoopSubCallReturnBehavior {
    let mut potential_file_name_str: Option<&str>;

    let mut is_directory = false;
    let mut file_name: String = String::new();
    let mut is_file_name_str_none = false;

    while *index < entries.len() && is_file_name_str_none && !is_directory {
        let entry = &entries[*index];
        let file_type = entry.file_type();

        let file_name_os = entry.file_name();
        let potential_file_name_str = match file_name_os.to_str() {
            None => None,
            Some(s) => Some(s),
        };

        is_file_name_str_none = potential_file_name_str.is_none();

        if file_type.is_err() {
            eprintln!("Error getting file type for entry: {:?}. Skipping", entry);
            *index += 1;
            continue;
        };

        is_directory = file_type.unwrap().is_dir();

        if is_file_name_str_none {
            eprintln!(
                "Error converting file name to string for entry: {:?}. Skipping",
                entry.file_name()
            );

            *index += 1;
            continue;
        }

        if !is_ok_name_according_to_filter(arguments, potential_file_name_str.unwrap(), directory_path) {
            *index += 1;
            continue;
        }

        file_name = potential_file_name_str.unwrap().to_string();
        *index += 1;
    }

    if *index >= entries.len() {
        return LoopSubCallReturnBehavior::Break;
    }

    if is_directory {
        return LoopSubCallReturnBehavior::Continue;
    }

    *file_group_length += file_name.len();
    file_group.push(file_name.clone());
    *index += 1;
    while *index < entries.len() {
        let entry = &entries[*index];
        let file_type = entry.file_type();
        if file_type.is_err() {
            eprintln!("Error getting file type for entry: {:?}. Skipping", entry);
            *index += 1;
            continue;
        };

        if file_type.unwrap().is_dir() {
            break;
        }

        let file_name_temporary = entry.file_name();
        potential_file_name_str = match file_name_temporary.to_str() {
            None => None,
            Some(s) => Some(s),
        };

        if potential_file_name_str.is_none() {
            eprintln!("Error getting file name for entry: {:?}. Skipping", entry);
            *index += 1;
            continue;
        }

        if !is_ok_name_according_to_filter(
            arguments,
            potential_file_name_str.unwrap(),
            directory_path,
        ) {
            *index += 1;
            continue;
        }

        file_name = potential_file_name_str.unwrap().to_string();
        file_group.push(file_name.clone());
        *file_group_length += file_name.len();
    }

    *file_group_length += 1;
    LoopSubCallReturnBehavior::NormalProcessing
}

fn is_ok_name_according_to_filter(
    arguments: &arguments::Arguments,
    file_name: &str,
    directory_path: &Path,
) -> bool {
    let result = arguments.file_check()(arguments, file_name);

    if !result {
        let full_path_buffer = directory_path.join(file_name);
        let full_path = full_path_buffer.to_str();
        if full_path.is_none() {
            eprintln!("Warning: a file who's name is not valid UTF-8: {:?} was skipped due to not matching the glob or regex pattern", file_name);
            return false;
        }

        let full_path = full_path_buffer.to_str().unwrap();
        eprintln!("Warning, the file '{}' was skipped due to not matching the glob or regex pattern", full_path);
    }

    result
}

fn sort_entries(entries: &mut Vec<DirEntry>, sorting_order: &SortingOrder) {
    entries.sort_by(|lhs_entry, rhs_entry| match sorting_order {
        SortingOrder::Alphabetical => lhs_entry.file_name().cmp(&rhs_entry.file_name()),
        SortingOrder::ReverseAlphabetical => {
            lhs_entry.file_name().cmp(&rhs_entry.file_name()).reverse()
        }
        SortingOrder::DateCreatedMostRecent => metadata_time_based_sort(
            lhs_entry,
            rhs_entry,
            "creation time",
            fs::Metadata::created,
            |lhs_time, rhs_time| lhs_time.cmp(rhs_time),
        ),
        SortingOrder::DateCreatedLeastRecent => metadata_time_based_sort(
            lhs_entry,
            rhs_entry,
            "creation time",
            fs::Metadata::created,
            |lhs_time, rhs_time| lhs_time.cmp(rhs_time).reverse(),
        ),
        SortingOrder::DateModifiedMostRecent => metadata_time_based_sort(
            lhs_entry,
            rhs_entry,
            "modification time",
            fs::Metadata::modified,
            |lhs_time, rhs_time| lhs_time.cmp(rhs_time),
        ),
        SortingOrder::DateModifiedLeastRecent => metadata_time_based_sort(
            lhs_entry,
            rhs_entry,
            "modification time",
            fs::Metadata::modified,
            |lhs_time, rhs_time| lhs_time.cmp(rhs_time).reverse(),
        ),
    });

    fn metadata_time_based_sort<TimeAcquirer, Sorter>(
        lhs_entry: &DirEntry,
        rhs_entry: &DirEntry,
        time_field_name: &str,
        time_acquirer: TimeAcquirer,
        sorter: Sorter,
    ) -> Ordering
    where
        TimeAcquirer: Fn(&fs::Metadata) -> std::io::Result<std::time::SystemTime>,
        Sorter: Fn(&std::time::SystemTime, &std::time::SystemTime) -> Ordering,
    {
        let lhs_metadata_result = lhs_entry.metadata();
        let rhs_metadata_result = rhs_entry.metadata();

        match (lhs_metadata_result, rhs_metadata_result) {
            (Ok(lhs_metadata), Ok(rhs_metadata)) => {
                let lhs_created_result = time_acquirer(&lhs_metadata);
                let rhs_created_result = time_acquirer(&rhs_metadata);

                match (lhs_created_result, rhs_created_result) {
                    (Ok(lhs_time), Ok(rhs_time)) => sorter(&lhs_time, &rhs_time),
                    (Ok(_), Err(rhs_time_error)) => {
                        eprintln!(
                            "Error: Fetching the {} for the file {} failed, therefore {} \
                            will be considered last in ordering compared to entries without issues\n\
                            The error that happened was {}",
                            &time_field_name,
                            &rhs_entry.path().display(),
                            &rhs_entry.path().display(),
                            rhs_time_error
                        );
                        Ordering::Greater
                    }
                    (Err(lhs_time_error), Ok(_)) => {
                        eprintln!(
                            "Error: Fetching the {} for the file {} failed, therefore {} \
                            will be considered last in ordering compared to entries without issues\n\
                            The error that happened was {}",
                            &time_field_name,
                            &lhs_entry.path().display(),
                            &lhs_entry.path().display(),
                            lhs_time_error
                        );
                        Ordering::Less
                    }
                    (Err(lhs_time_error), Err(rhs_time_error)) => {
                        eprintln!(
                            "Error: Fetching the {} for the files {} and {} failed, therefore they \
                             will be considered equal but last overall in ordering compared to entries without issues\n\
                             The errors that happened were \n{} and \n{} respectively.",
                            &time_field_name,
                            &lhs_entry.path().display(),
                            &rhs_entry.path().display(),
                            lhs_time_error,
                            rhs_time_error
                        );
                        Ordering::Equal
                    }
                }
            }
            (Ok(_), Err(rhs_metadata_error)) => {
                eprintln!(
                    "Error: Fetching the metadata for the file {} failed, therefore {} will be considered last in ordering compared to entries without issues\n\
                    The error that happened was {}",
                    &rhs_entry.path().display(),
                    &rhs_entry.path().display(),
                    rhs_metadata_error
                );
                Ordering::Greater
            }
            (Err(lhs_metadata_error), Ok(_)) => {
                eprintln!(
                    "Error: Fetching the metadata for the file {} failed, therefore {} will be considered last in ordering compared to entries without issues\n\
                    The error that happened was {}",
                    &lhs_entry.path().display(),
                    &lhs_entry.path().display(),
                    lhs_metadata_error
                );
                Ordering::Less
            }
            (Err(lhs_metadata_error), Err(rhs_metadata_error)) => {
                eprintln!(
                    "Error: Fetching the metadata for the files {} and {} failed, therefore {} amd {} will be considered equal in ordering compared to entries without issues\n\
                    The errors that happened were \n{} and \n{} respectively",
                    &lhs_entry.path().display(),
                    &rhs_entry.path().display(),
                    &lhs_entry.path().display(),
                    &rhs_entry.path().display(),
                    lhs_metadata_error,
                    rhs_metadata_error
                );
                Ordering::Equal
            }
        }
    }
}

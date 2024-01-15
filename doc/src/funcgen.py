

def check_addr_tags(s):
    if s in ["null", "ret0", "macro"]:
        return f"none[^{s}]"
    return s

def check_decomp_tags(s):
    if s in ["crt"]:
        return f"no[^crt]"
    return s

def gen_table_line(parts, widths):
    assert len(parts) == len(widths)
    res = []
    for i in range(len(parts)):
        res.append(f"%-{widths[i]}s" % parts[i])
    return "| %s |" % " | ".join(res)


# read CSV
with open("funcs.csv") as fp:
    lines = [line.strip().split(",") for line in fp]

# make sure all lines have the same number of fields
for line in lines:
    if len(line) != len(lines[0]):
        raise ValueError("Invalid CSV")

# strip off first line for header
hdr, *lines = lines

# process (remaining) lines
new_lines = []
for name, pc1_0_addr, pc1_6_addr, slus_addr, group, subgroup, decomp in lines:
    # wrap name in backticks
    name = f"`{name}`"
    # check for footnote tags
    pc1_0_addr = check_addr_tags(pc1_0_addr)
    pc1_6_addr = check_addr_tags(pc1_6_addr)
    slus_addr = check_addr_tags(slus_addr)
    decomp = check_decomp_tags(decomp)
    new_lines.append([name, pc1_0_addr, pc1_6_addr, slus_addr, group, subgroup, decomp])
lines = new_lines

# compute column widths
widths = [len(field) for field in hdr]
for line in lines:
    for i, field in enumerate(line):
        widths[i] = max(widths[i], len(field))


# generate table
# header
print(gen_table_line(hdr, widths))
# header separator
print(gen_table_line([""] * len(widths), widths).replace(" ", "-"))
# remaining lines
for line in lines:
    print(gen_table_line(line, widths))

import os
import sys
import pickle

def pbin_pack():
	"""
		Pack panorama folder to buwq.pbin
		Return codes:
			0 - no errors
			1 - code.pbin.table not found (unpack original code.pbin first)
			2 - modified file larger than original
	"""

	output_file_content = b""
	output_file_content += b"\x50\x41\x4E\x02"
	output_file_content += b'\x00' * 512

	try:
		with open("code.pbin.table", "rb") as f:
			file_table = pickle.load(f)
	except FileNotFoundError:
		return 1

	for root, _, files in os.walk("panorama"):
		for file in files:
			file_name = os.path.join(root, file).replace("/", "\\")
			file_size = os.stat(file_name).st_size

			if file_name not in file_table:
				print(f"{file_name} : Unknown file")
				continue

			file_original_size = file_table[file_name]

			output_file_content += b"\x50\x4B\x03\x04\x0A\x00\x00\x00\x00\x00\x00\x00\x00\x00\x82\xC2\xA9\x51"
			output_file_content += file_original_size.to_bytes(4, byteorder="little")
			output_file_content += file_original_size.to_bytes(4, byteorder="little")
			output_file_content += len(file_name).to_bytes(4, byteorder="little")
			output_file_content += file_name.encode("utf-8")

			with open(file_name, "rb") as f:
				output_file_content += f.read()
				if file_size > file_original_size:
					print(f"{file_name} : {file_size} B > {file_original_size} B")
					return 2
				output_file_content += b'\x20' * (file_original_size - file_size)

	if "__CODE_PBIN_END__" not in file_table:
		return 1
	output_file_content += file_table["__CODE_PBIN_END__"]

	while True:
		try:
			with open("buwq.pbin", "wb") as f:
				f.write(output_file_content)
		except PermissionError:
			print("The file is already in use by some program. Close unnecessary programs and try again.")
			input("Press enter to try again... ")
			continue
		break

	return 0

if __name__ == "__main__":
	code = pbin_pack()

	match code:
		case 1:
			print("'code.pbin.table' not found. Maybe you should unpack your original code.pbin first?")

		case 2:
			print(f"The modified file size is larger than the original one.")

	sys.exit(code)

from PIL import Image

image = Image.open("finch.png")
pixels = image.load()

out_file = open("finch.bin", "wb")
counter = 0

correction_table = [
	0x0, 0x1, 0xC, 0xF,
	0xE, 0xD, 0x6, 0x4,
	0x8, 0x3, 0xA, 0x7,
	0x9, 0x2, 0x5, 0xB,
]

for y in range(4096):
	for x in range(0, 256, 2):
		byte = b""
		source = ""
		try:
			low_nib = pixels[x, y]
			high_nib = pixels[x + 1, y]
			try:
				low_nib = correction_table[low_nib]
				high_nib = correction_table[high_nib]
			except IndexError:
				print(f"Invalid pixel value! low_nib={low_nib} high_nib={high_nib}")
				raise Exception()
			source = "image"
			byte = chr(low_nib | (high_nib << 4)).encode("raw_unicode_escape")
		except IndexError:
			source = "zero"
			byte = chr(0).encode("raw_unicode_escape")
		out_file.write(byte)
		counter += 1
		if (counter != out_file.tell()):
			print(f"byte={byte} from {source} at ({x}, {y})")
			print(f"counter={counter} curr={out_file.tell()}")
			raise Exception("python pls")

print(counter)
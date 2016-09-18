import copy
import argparse
import json

parser = argparse.ArgumentParser()
parser.add_argument("input", help="Path to input file")
parser.add_argument("output", help="Path to output file")
args = parser.parse_args()

valid_properties = {'fps', 'repeat', 'period'}
properties = {}

result_table = {}
current_animation = None


def construct_base_frame():
    current_frame = copy.deepcopy(current_animation['effect'])
    current_frame['period'] = 1/properties['fps']
    return current_frame


def finalize_current_animation():
    global current_animation
    if current_animation:
        result_table[current_animation['name']] = {
            'frames': current_animation['frames'],
            'repeat': current_animation['repeat'],
        }
        current_animation = None


def start_new_animtion(animation_name):
    properties['fps'] = 10.0
    properties['compose'] = False
    global current_animation
    current_animation = {
        'name': animation_name,
        'effect': {},
        'repeat': 'loop',
        'frames': [],
    }


def parse_data(input):
    finalize_current_animation()
    start_new_animtion(input)


def parse_property(input):
    split = input.split(' ', 1)
    name = split[0]
    if name not in valid_properties:
        raise Exception("Unknown property name: " + name)

    if name == 'repeat':
        current_animation['repeat'] = split[1]
    elif len(split) > 1:
        if name == "period":
            properties["fps"] = 1/float(split[1])
        else:
            properties[name] = float(split[1])
    else:
        properties[name] = True


def transform_color(s):
    x = int(s, 16)
    return (x >> 16) % 256, (x >> 8) % 256, x % 256, 1.0

valueparsers = {
    'name': lambda x: x,
    'color': transform_color,
    'alpha': lambda x: float(x),
    'position': lambda x: list(map(float, x.split(' '))),
    'size': lambda x: map(float, x.split(' ')),
    'rotation': lambda x: float(x),
    'mirror': lambda x: x,
    'period': lambda x: float(x),
}


def parse_ring(input):
    split = input.split(' ', 1)
    name = split[0]

    current_frame = construct_base_frame()

    import re
    for mod in re.findall(r"\[(.*?)\]", split[1]):
        mod_name, mod_value = mod.split(' ', 1)
        try:
            value_parser = valueparsers[mod_name]
        except KeyError as e:
            raise Exception("Unknown frame field: {}".format(e))
        current_frame[mod_name] = value_parser(mod_value)

    if 'alpha' in current_frame:
        if 'color' not in current_frame:
            current_frame['color'] = [1.0, 1.0, 1.0, 1.0]
        current_frame['color'][3] = current_frame['alpha']
        del current_frame['alpha']

    if 'mirror' in current_frame:
        if current_frame['mirror'].find('h') > -1:
            current_frame['mirrorh'] = True
        if current_frame['mirror'].find('v') > -1:
            current_frame['mirrorv'] = True
        del current_frame['mirror']

    if name == "effect":
        if properties['compose']:
            raise Exception("NYI")
        else:
            for k in current_frame:
                current_animation['effect'][k] = current_frame[k]

    elif name == "frame":
        current_animation['frames'].append(current_frame)


def parse_chain(input):
    split = input.split(' ', 1)
    name = split[0]
    assert(name == "name")

    for name in split[1].split(' '):
        current_frame = construct_base_frame()
        current_frame['name'] = name
        current_animation['frames'].append(current_frame)

syntax_mapping = {
    '#': lambda x: None,
    '$': parse_data,
    '@': parse_property,
    '+': parse_ring,
    '%': parse_chain
}

line_num = 0
for line in open(args.input):
    line_num += 1
    command = line.strip()
    if not command:
        continue
    try:
        syntax_mapping[command[0]](command[1:])
    except Exception as err:
        print("Error: {0}:{1} - {2}".format(args.input, line_num, str(err)))
        raise SystemExit(1)

finalize_current_animation()

with open(args.output, 'w') as output_file:
    json.dump(result_table,
              output_file,
              sort_keys=True,
              indent=4,
              separators=(',', ': '))

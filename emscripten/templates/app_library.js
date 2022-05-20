mergeInto(LibraryManager.library, {
    getLocalFile: async function() {
        // const pickerOpts = {
        //     types: [
        //         {
        //         description: 'Text',
        //         accept: {
        //             'text/*': ['.txt', '.log']
        //         }
        //         },
        //     ],
        //     excludeAcceptAllOption: true,
        //     multiple: false
        // };
        try {
            // Always returns an array.
            const [handle] = await window.showOpenFilePicker({
                types: [], // default
                multiple: false, // default
                excludeAcceptAllOption: false, // default
                _preferPolyfill: false // default
              });
            const file = await handle.getFile();
            const contents = await file.arrayBuffer();
            var uint8View = new Uint8Array(contents);
            Module.open_file(file.name, uint8View, contents.byteLength);
            //result = Module.ccall("int_sqrt", null, ["string", "array", "number"], [file.name, uint8View, contents.byteLength])

            console.log(file);
            
            return 0;
    
            // return handle.getFile();
        } catch (err) {
            console.error(err.name, err.message);
            Module.messageErr(err.message);
        } finally {
            // To avoid memory leaks we need to always clear out the allocated heap data
            // This needs to happen in the finally block, otherwise thrown errors will stop code execution before this happens
            //Module._free(buffer)
        }
        // Do something with the file handle.

        return 1;
    },
    saveLocalFile: async function() {
        try {
            var myUint8Array = Module.getBytes();
            //var blob = new Blob(myUint8Array, {type: "text/json"});
            var string = new TextDecoder().decode(new Uint8Array(myUint8Array));
            // create a new handle
            handle = await window.showSaveFilePicker({
                suggestedName: 'preset.json'
            });

            // create a FileSystemWritableFileStream to write to
            const writableStream = await handle.createWritable();

            await writableStream.write(string);
            await writableStream.close();

            // write our file
            //await writableStream.write(myUint8Array);
            //console.log(blob);

            // close the file and write the contents to disk.
            // await writableStream.close();
            // Module.message("Saving file: " + handle.name);
        } catch (err) {
            console.error(err.name, err.message);
            Module.messageErr(err.message);
        } finally {
            // To avoid memory leaks we need to always clear out the allocated heap data
            // This needs to happen in the finally block, otherwise thrown errors will stop code execution before this happens
            //Module._free(buffer)
        }
    },
});
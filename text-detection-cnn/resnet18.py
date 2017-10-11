from resnet import ResnetBuilder
from data import img_size, train_generator, validation_generator, steps_per_epoch, validation_steps, epochs

model = ResnetBuilder.build_resnet_18((1, img_size, img_size), 1)
model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])
model.fit_generator(train_generator,
                    steps_per_epoch=steps_per_epoch,
                    epochs=epochs,
                    validation_data=validation_generator,
                    validation_steps=validation_steps)
model.save('resnet18.h5')
